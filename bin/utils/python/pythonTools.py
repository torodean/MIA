from datetime import datetime

# This is a dictionary of all color codes supported.
# Important: Make sure the desired default value is first.
colorCodes = {
    "text":    ("default", "\033[0m"),       # Reset color
    "warning": ("yellow",  "\033[93m"),      # Yellow - Warning text
    "error":   ("red",     "\033[91m"),      # Red - Error text
    "note":    ("blue",    "\033[94m"),      # Blue - Notes or program information
    "success": ("green",   "\033[92m"),      # Green - Success text
    "command": ("cyan",    "\033[36m"),      # Cyan - Command output text
    "program": ("magenta", "\033[35m"),      # Magenta - Program-specific output
    "debug":   ("gray",    "\033[90m"),      # Gray - Debug/detail output
    "test":    ("orange",  "\033[38;5;208m") # Orange - Testing
}


def get_current_time():
    """
    Get the current date and time.

    Returns:
        str: The current date and time in the format YYYYMMDD HH:MM:SS.
    """
    return datetime.now().strftime("%Y%m%d %H:%M:%S")


def append_text_to_file(text, logFile):
    """
    Append text to a file.

    Args:
        text (str): The text to append to the file.
        logFile (str): The path to the file.

    Returns:
        None
    """
    with open(logFile, "a") as file:
        file.write(f"{text}\n")


def output_text(text, option=None, color=None, addLabel=False, logFile=None):
    """
    Print text to the console in a specified color using ANSI escape codes. 
    This will also optionally append a label to the output text based on what type it is.

    Args:
        text (str): The text to be printed.
        option (str, optional): The option for the text. See the colorCodes variable for 
            the defined values. Invalid options result in default text.
        color (str, optional): The color value for the text. This value will only be used if
            if an 'option' is not specified.
        addLabel (bool, optional): Whether or not to add an appropriate label to the output 
            message. A quick check will first be done to see if the label already exists - 
            in which case it will be skipped.
        logFile (str, optional): An optional log file to also output the text to. If a log
            file is specified, this will also append a timestamp to the message in the format 
            of "[YYYYMMDD HH:MM:SS] text".

    Returns:
        None

    Note:
        This function uses ANSI escape codes for color formatting. Colors may not display correctly 
        in all environments (e.g., some IDEs or Windows terminals without ANSI support).
    """
    
    text = str(text)  # Ensure text is a string.
      
    # Error case for entering an invalid color. 
    if option is not None and option not in colorCodes:
        output_text(f"Option '{option}' not found. Using default text color.", option="warning", addLabel=True)
    
    # Check input parameters to determine formatting to use.
    if option in colorCodes:
        label = None if option == list(colorCodes.keys())[0] else option.upper()
        code = colorCodes[option][1]
    elif color is not None:
        label = None
        code = list(colorCodes.values())[0][1]
        colorFound = False

        for option in colorCodes:
            if colorCodes[option][0].lower() == str(color).lower():
                label = colorCodes[option][0]
                code = colorCodes[option][1]
                colorFound = True
                break
                
        # Error case for entering an invalid color.
        if not colorFound:
            output_text(f"Color '{color}' not found. Using default text color.", option="warning", addLabel=True)
        
    else: # Default to the first color code.
        label = None
        code = list(colorCodes.values())[0][1]
    
    # Check for existing label at start of text and append if needed.
    if addLabel and label is not None:
        if not text.lower().startswith(f"{label.lower()}:"):
            text = f"{label.upper()}: {text}"
    
    # Optionally write to log file.
    if logFile is not None:
        timestampedText = f"[{get_current_time()}] {text}"
        append_text_to_file(timestampedText, logFile)

    # Output appropriate text with appropriate color.
    resetCode = list(colorCodes.values())[0][1]
    print(f"{code}{text}{resetCode}")


def main():
    # Default text
    output_text("This is default text.")
    
    # Each defined option
    output_text("\n-- Here is some text with just the option specified.")
    output_text("This is a warning.", option="warning")
    output_text("This is an error.", option="error")
    output_text("This is a note.", option="note")
    output_text("This is a success message.", option="success")
    output_text("This is command output.", option="command")
    output_text("This is program output.", option="program")
    output_text("This is debug output.", option="debug")
    output_text("This is test output.", option="test")

    # Labels
    output_text("\n-- Here is some text with the option and labels specified.")
    output_text("Something went wrong.", option="error", addLabel=True)
    output_text("Something may be wrong.", option="warning", addLabel=True)
    output_text("Everything worked.", option="success", addLabel=True)

    # Existing labels
    output_text("\n-- Here is some text with the option and labels specified but also with existing labels.")
    output_text("ERROR: Something went wrong.", option="error", addLabel=True)
    output_text("Warning: Something may be wrong.", option="warning", addLabel=True)

    # Color specified by color name
    output_text("\n-- Here is some text with just the color specified.")
    output_text("Yellow text.", color="Yellow")
    output_text("Red text.", color="Red")
    output_text("Orange text.", color="Orange")

    # Invalid option should use the default color
    output_text("\n-- Here is some text with invalidoptions and colors specified.")
    output_text("Invalid option.", option="invalid")

    # Invalid color should use the default color
    output_text("Invalid color.", color="Purple")

    # Option takes precedence over color
    output_text("\n-- Here is some text with option (success) and a color (red) specified.")
    output_text("Option takes precedence.", option="success", color="Red")


if __name__ == "__main__":
    main()
