#!/usr/bin/env python3
import tkinter as tk


class GameUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Game")
        self.root.geometry("800x640")
        self.root.minsize(800, 640)

        self.create_ui()

    def create_ui(self):
        # Configure the main window.
        self.root.grid_rowconfigure(1, weight=1)
        self.root.grid_columnconfigure(0, weight=1)

        # Top frame.
        self.top_frame = tk.Frame(self.root)
        self.top_frame.grid(
            row=0,
            column=0,
            sticky="ew",
            padx=5,
            pady=5
        )

        # Placeholder top buttons.
        for i in range(5):
            button = tk.Button(
                self.top_frame,
                text=f"Button {i + 1}",
                width=12,
                height=2
            )
            button.pack(side="left", padx=3)

        # Game display.
        self.game_display = tk.Frame(
            self.root,
            bg="black"
        )
        self.game_display.grid(
            row=1,
            column=0,
            sticky="nsew",
            padx=5,
            pady=5
        )

        # Bottom frame.
        self.bottom_frame = tk.Frame(self.root)
        self.bottom_frame.grid(
            row=2,
            column=0,
            sticky="ew",
            padx=5,
            pady=5
        )

        # Bottom buttons.
        button_labels = [
            "1", "2", "3", "4", "5", "6",
            "7", "8", "9", "0", "-", "="
        ]

        for i, label in enumerate(button_labels):
            button = tk.Button(
                self.bottom_frame,
                text=label,
                width=4,
                height=2
            )
            button.grid(
                row=0,
                column=i,
                padx=2,
                pady=2
            )


if __name__ == "__main__":
    root = tk.Tk()
    app = GameUI(root)
    root.mainloop()