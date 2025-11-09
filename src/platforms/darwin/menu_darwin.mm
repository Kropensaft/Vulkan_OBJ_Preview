#include "menu_darwin.h" // Include the C header we just created
#include <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#include <objc/runtime.h>

// A simple helper class to act as the target for the menu item's action.
// This is a cleaner approach than using global state.
@interface MenuActionTarget : NSObject
@property(nonatomic, assign) OpenFileCallback openFileCallback;
- (void)openFileAction:(id)sender;
@property(nonatomic, assign) RenderWireframeCallback toggleWireframeCallback;
@end

@implementation MenuActionTarget

- (void)openFileAction:(id)sender {
  if (self.openFileCallback == nullptr) {
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
@end

// This is the C function implementation that our C++ code will call.
// This is a special key we use to find our object later.
// Its address is what's important, not its value.
static char kMenuTargetKey;

void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback open_callback,
                           RenderWireframeCallback wireframe_callback) {
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

    // --- Create a NEW, LOCAL MenuActionTarget instance ---
    MenuActionTarget *menuTarget = [[MenuActionTarget alloc] init];
    menuTarget.openFileCallback = open_callback;
    menuTarget.toggleWireframeCallback = wireframe_callback;

    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:appMenuItem];
    NSMenu *appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];

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
    [viewMenu addItem:renderWireframeItem];
    [app setMainMenu:mainMenu];
  }
}
