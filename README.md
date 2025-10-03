# Popup Clicker

A simple Windows utility to automatically click buttons in popup dialogs.

## What it is

`popup_clicker` is a lightweight C++ application that runs in the background and monitors for the appearance of specific popup windows. When a window with a matching title appears, it will automatically click the desired button. This is useful for automating repetitive confirmation dialogs.

## Configuration

The behavior of `popup_clicker` is controlled by the `popup_clicker.ini` file. This file must be in the same directory as the executable.

The configuration file supports the following keys under the `[popup]` section (or with no section):

*   `targetTitle`: The title of the popup window to target.
*   `buttonText`: The text of the button to click within the target window.

**Example `popup_clicker.ini`:**

```ini
[popup]
targetTitle=Warning
buttonText=Yes
```

If `popup_clicker.ini` is not found, the application will use the default values: `targetTitle=Warning` and `buttonText=OK`.

## Building from Source

To compile the application from source, you will need a C++ compiler like g++.

1.  Make sure you have `g++` installed and in your system's PATH.
2.  Run the `compile.ps1` PowerShell script:

    ```powershell
    .\compile.ps1
    ```

This will create `popup_clicker.exe` in the current directory.

## Testing

A PowerShell script, `test.ps1`, is provided to help verify the functionality of `popup_clicker`.

1.  Make sure `popup_clicker.exe` is running.
2.  Run the `test.ps1` script from a PowerShell terminal:

    ```powershell
    .\test.ps1
    ```

This script will display a "Warning" message box with "Yes" and "No" buttons. If `popup_clicker` is configured correctly (as in the example above), it should automatically click the "Yes" button, and the script will output "You clicked: Yes".