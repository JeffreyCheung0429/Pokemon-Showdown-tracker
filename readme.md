# Showdown Recorder

This project is a minimum Showdown battle recorder that connects to a WebSocket server, listens for battle logs, and saves them to text files. It also includes a utility to convert these logs into HTML files.

## Files

### `main.py`

This is the main script that connects to the WebSocket server, listens for battle logs, and saves them to text files.

### `utils.py`

This file contains utility functions used by `main.py` for parsing and processing the WebSocket messages.

### `makeLengLengHTML.c`

This C program converts the battle logs saved in text files to HTML files.

### `config.ini`

Configuration file for the project.

### `logs/`

Directory where the battle logs are saved.

## Usage

### Running the Recorder

1. Ensure you have Python and the required dependencies installed.
1. Configure the `config.ini` file with the appropriate settings.
1. Run the recorder:

```python main.py```

### Converting Logs to HTML

1. Compile the C program `makeLengLengHTML.c` if not already compiled:

```gcc makeLengLengHTML.c -o makeLengLengHTML```

1. Run the program with the log file as an argument:

```./makeLengLengHTML logs/battle-gen9vgc2025regg-1234567890.txt```

This will generate an HTML file with the same name as the log file.

## Configuration

The `config.ini` file should contain the following settings:

```ini
[settings]
target = player1, player2
format = gen9vgc2025
url = wss://sim3.psim.us/showdown/websocket
```

- `target`: Comma-separated list of target players.
- `format`: Format of the battles to record.
- `url`: WebSocket URL to connect to.
