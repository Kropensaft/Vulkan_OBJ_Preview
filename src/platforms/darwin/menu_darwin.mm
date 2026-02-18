#include "menu_darwin.h" // Include the C header we just created
#include <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#include <cstddef>
#include <objc/runtime.h>

// A simple helper class to act as the target for the menu item's action.
// This is a cleaner approach than using global state.
@interface MenuActionTarget : NSObject
@property(nonatomic, assign) OpenFileCallback openFileCallback;
- (void)openFileAction:(id)sender;
@property(nonatomic, assign) RenderWireframeCallback toggleWireframeCallback;
@property(nonatomic, assign) ZoomInCallback zoom_inCallback;
@property(nonatomic, assign) ZoomOutCallback zoom_outCallback;
@property(nonatomic, assign) SwitchLightSourceCallback ls_callback;
@property(nonatomic, assign) SetZoomSensitivityCallback zoom_sens_callback;
@end

@implementation MenuActionTarget

- (void)openFileAction:(id)sender {
  NSLog(@"'openFileAction' has been triggered.");
  if (self.openFileCallback == nullptr) {
    NSLog(@"ERROR: The openFileCallback is null. No action will be taken.");
    return;
  }

  NSOpenPanel *panel = [NSOpenPanel openPanel];
  [panel setCanChooseFiles:YES];
  [panel setAllowsMultipleSelection:NO];
  [panel setAllowedFileTypes:@[ @"obj" ]];

  if ([panel runModal] == NSModalResponseOK) {
    NSString *path = [[panel URL] path];
    // Call the C++ function pointer that was passed to us
    self.openFileCallback([path UTF8String]);
  }
}

- (void)toggleWireframeAction:(id)sender {
  if (self.toggleWireframeCallback == nullptr) {
    return;
  } else {
    self.toggleWireframeCallback();
  }
}

- (void)zoomInAction:(id)sender {
  if (self.zoom_inCallback == nullptr) {
    return;
  } else {
    self.zoom_inCallback();
  }
}

- (void)zoomOutAction:(id)sender {
  if (self.zoom_outCallback == nullptr) {
    return;
  } else {
    self.zoom_outCallback();
  }
}

- (void)ls_switchAction:(id)sender {
  if (self.ls_callback == nullptr) {
    return;
  } else {
    self.ls_callback();
  }
}

- (void)zoom_sensAction:(NSSlider *)sender {

  double sensitivity = [sender doubleValue];
  if (self.zoom_sens_callback == nullptr) {
    return;
  } else {
    self.zoom_sens_callback(sensitivity);
  }
}

@end

// This is the C function implementation that our C++ code will call.
// This is a special key we use to find our object later.
// Its address is what's important, not its value.
static char kMenuTargetKey;

void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback open_callback,
                           RenderWireframeCallback wireframe_callback,
                           ZoomInCallback zoomIn_callback,
                           ZoomOutCallback zoomOut_callback,
                           SwitchLightSourceCallback ls_callback,
                           SetZoomSensitivityCallback zoom_sens_callback) {
  // Cast the C++ void pointer back to a native NSWindow pointer
  NSWindow *window = (__bridge NSWindow *)native_window_handle;
  if (!window) {
    return;
  }

  // Check if we've already attached a target to this window
  if (objc_getAssociatedObject(window, &kMenuTargetKey) != nil) {
    return; // Menu already created for this window
  }

  @autoreleasepool {
    NSApplication *app = [NSApplication sharedApplication];
    NSMenu *mainMenu = [[NSMenu alloc] init];

    MenuActionTarget *menuTarget = [[MenuActionTarget alloc] init];
    menuTarget.openFileCallback = open_callback;
    menuTarget.toggleWireframeCallback = wireframe_callback;
    menuTarget.zoom_inCallback = zoomIn_callback;
    menuTarget.zoom_outCallback = zoomOut_callback;
    menuTarget.ls_callback = ls_callback;
    menuTarget.zoom_sens_callback = zoom_sens_callback;

    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:appMenuItem];
    NSMenu *appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];

    NSMenuItem *quitMenuItem =
        [[NSMenuItem alloc] initWithTitle:@"Quit"
                                   action:@selector(terminate:)
                            keyEquivalent:@"q"];
    [quitMenuItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
    [appMenu addItem:quitMenuItem];

    NSMenuItem *fileMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:fileMenuItem];
    NSMenu *fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
    [fileMenuItem setSubmenu:fileMenu];
    objc_setAssociatedObject(window, &kMenuTargetKey, menuTarget,
                             OBJC_ASSOCIATION_RETAIN);

    // --- File Menu ---
    NSMenuItem *openItem =
        [[NSMenuItem alloc] initWithTitle:@"Open .obj File..."
                                   action:@selector(openFileAction:)
                            keyEquivalent:@"o"];

    [openItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
    [openItem setTarget:menuTarget];
    [fileMenu addItem:openItem];

    NSMenuItem *viewMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:viewMenuItem];
    NSMenu *viewMenu = [[NSMenu alloc] initWithTitle:@"View"];
    [viewMenuItem setSubmenu:viewMenu];

    // --- View Menu ---
    NSMenuItem *renderWireframeItem =
        [[NSMenuItem alloc] initWithTitle:@"Render wireframe mesh only"
                                   action:@selector(toggleWireframeAction:)
                            keyEquivalent:@"w"];
    [renderWireframeItem setTarget:menuTarget];

    NSMenuItem *LsSwitchItem =
        [[NSMenuItem alloc] initWithTitle:@"Switch light source to current view"
                                   action:@selector(ls_switchAction:)
                            keyEquivalent:@"s"];

    [LsSwitchItem setTarget:menuTarget];

    NSMenuItem *zoomInItem =
        [[NSMenuItem alloc] initWithTitle:@"Zoom In"
                                   action:@selector(zoomInAction:)
                            keyEquivalent:@"+"];
    [zoomInItem setTarget:menuTarget];

    NSMenuItem *zoomOutItem =
        [[NSMenuItem alloc] initWithTitle:@"Zoom Out"
                                   action:@selector(zoomOutAction:)
                            keyEquivalent:@"-"];
    [zoomOutItem setTarget:menuTarget];

    NSMenuItem *sliderMenuItem = [[NSMenuItem alloc] init];
    NSView *containerView =
        [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 250, 55)];

    //  Minus Magnifying Glass (Left)
    NSImage *minIcon =
        [NSImage imageWithSystemSymbolName:@"minus.magnifyingglass"
                  accessibilityDescription:nil];
    NSImageView *minIconView = [NSImageView imageViewWithImage:minIcon];
    [minIconView setFrame:NSMakeRect(15, 5, 20, 20)];
    [minIconView setContentTintColor:[NSColor secondaryLabelColor]];
    [containerView addSubview:minIconView];
    NSTextField *titleLabel = [NSTextField labelWithString:@"Zoom Sensitivity"];
    [titleLabel setFrame:NSMakeRect(15, 30, 220, 20)];
    [titleLabel setFont:[NSFont menuFontOfSize:14.0]];
    [titleLabel setTextColor:[NSColor labelColor]];
    [containerView addSubview:titleLabel];

    // Slider Control
    NSSlider *slider = [NSSlider sliderWithTarget:menuTarget
                                           action:@selector(zoom_sensAction:)];

    [slider setFrame:NSMakeRect(40, 5, 170, 20)];
    [slider setMinValue:ZOOM_SLIDER_MIN];
    [slider setMaxValue:ZOOM_SLIDER_MAX];
    [slider setDoubleValue:1.0];
    [slider setContinuous:YES];
    [containerView addSubview:slider];

    NSImage *maxIcon =
        [NSImage imageWithSystemSymbolName:@"plus.magnifyingglass"
                  accessibilityDescription:nil];
    NSImageView *maxIconView = [NSImageView imageViewWithImage:maxIcon];
    [maxIconView setFrame:NSMakeRect(215, 5, 20, 20)];
    [maxIconView setContentTintColor:[NSColor secondaryLabelColor]];
    [containerView addSubview:maxIconView];

    [sliderMenuItem setView:containerView];

    [viewMenu addItem:renderWireframeItem];
    [viewMenu addItem:zoomInItem];
    [viewMenu addItem:zoomOutItem];
    [viewMenu addItem:LsSwitchItem];
    [viewMenu addItem:[NSMenuItem separatorItem]];
    [viewMenu addItem:sliderMenuItem];
    [app setMainMenu:mainMenu];
  }
}
