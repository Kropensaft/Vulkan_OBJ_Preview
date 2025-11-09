#include "menu_darwin.h" // Include the C header we just created
#import <Cocoa/Cocoa.h>

// A simple helper class to act as the target for the menu item's action.
// This is a cleaner approach than using global state.
@interface MenuActionTarget : NSObject
@property(nonatomic, assign) OpenFileCallback openFileCallback;
- (void)openFileAction:(id)sender;
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
@end

// Keep a static instance of our target so it doesn't get deallocated
static MenuActionTarget *menuTarget = nil;

// This is the C function implementation that our C++ code will call.
void create_macos_menu_bar(void *native_window_handle,
                           OpenFileCallback callback) {
  // This function can be called only once.
  if (menuTarget != nil) {
    return;
  }

  @autoreleasepool {
    // Get the shared NSApplication instance created by GLFW
    NSApplication *app = [NSApplication sharedApplication];

    // Create the main menu bar container
    NSMenu *mainMenu = [[NSMenu alloc] initWithTitle:@"Main Menu"];

    // --- Application Menu ---
    NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:appMenuItem];
    NSMenu *appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];
    [appMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Quit"
                                                action:@selector(terminate:)
                                         keyEquivalent:@"q"]];

    // --- File Menu ---
    NSMenuItem *fileMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:fileMenuItem];
    NSMenu *fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
    [fileMenuItem setSubmenu:fileMenu];

    // --- "Open" Item ---
    // Create our target and store the C++ callback in it
    menuTarget = [[MenuActionTarget alloc] init];
    menuTarget.openFileCallback = callback;

    NSMenuItem *openItem =
        [[NSMenuItem alloc] initWithTitle:@"Open .obj File..."
                                   action:@selector(openFileAction:)
                            keyEquivalent:@"o"];
    [openItem setTarget:menuTarget];
    [fileMenu addItem:openItem];

    // Set the completed menu as the application's main menu
    [app setMainMenu:mainMenu];
  }
}
