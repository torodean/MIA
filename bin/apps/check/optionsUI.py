#!/usr/bin/env python3
"""
@file    optionsUI.py
@author  Antonius Torode
@date    11/02/2026

This file defines the options/game-settings panel for the CHECK UI.

The panel opens inside the main window to the right of the board. A thin bar,
(a collapse tab) is attached to the board's right edge. Clicking it opens the
panel, and clicking it again closes the panel.

The panel shows a turn indicator and the move history. Buttons added
to the panel send their events through the addEvent callable given at
construction. Those events join the board's own events in the same queue,
and the C++ backend picks them up with its usual polling. This module
does not imports checkUI, so it can be ran and tested by itself.
"""
import tkinter as tk
from tkinter import ttk

#########################
# CONFIGURATION VARIABLES
#########################

# Pixel width of the opened panel, and border width between it and the
# collapse bar. The width fits the move number column, two move columns
# wide enough for a full move line, the copy button, the scrollbar and the
# rules.
PANEL_WIDTH = 280
PANEL_BORDER = 1

# Colors matching the checkUI palette.
BACKGROUND = "#ffddc9"
LABEL_FG = "#333333"
LIGHT = "#bfa08e"
DARK = "#7d6455"
SELECTED_HIGHLIGHT = "#F7EC59"

# Fonts for the turn indicator and the move history boxes.
KING_FONT = ("Segoe UI Symbol", 30)
TURN_FONT = ("Segoe UI", 12, "bold")
MOVE_FONT = ("DejaVu Sans Mono", 10)

# Arrows on the collapse bar. The right arrow shows while the panel is
# closed, the left arrow while it is open.
BAR_COLLAPSED = "\u00bb"  # Right-pointing double angle quote.
BAR_OPEN = "\u00ab"       # Left-pointing double angle quote.

##############
# OPTIONS PANEL
##############

class OptionsPanel:
    """
    The collapse bar plus the panel it shows and hides. The bar always sits
    beside the board. The panel opens to the right of the bar and widens
    the window until it is closed again.
    """

    def __init__(self, master, addEvent, glyphs):
        """
        Builds the collapse bar and the panel, beside whatever is already
        packed in the master frame.

        @param master The frame the bar and the panel are packed into. The
               board must be packed to its left already.
        @param addEvent A callable which queues a UI event string. Buttons
               on the panel send their events through it, so this module never
               needs to know about checkUI.
        @param glyphs The dict mapping piece names to their glyphs. The
               panel keeps this dict by reference. Pieces are registered after
               the panel is built, so updateGlyphs() re-reads the dict whenever
               a piece appears.
        """
        self.addEvent = addEvent
        self.glyphs = glyphs
        self.visible = False

        # The collapse tab, always visible against the board's right edge.
        self.bar = tk.Button(
            master,
            text=BAR_COLLAPSED,
            command=self.toggle,
            relief=tk.FLAT,
            bd=0,
            highlightthickness=0,
            takefocus=0,
            cursor="hand2",
            bg=BACKGROUND,
            activebackground=BACKGROUND,
            fg=LABEL_FG,
        )
        self.bar.pack(side=tk.LEFT, fill=tk.Y)

        # The panel itself, hidden until first shown.
        self.frame = tk.Frame(
            master,
            width=PANEL_WIDTH,
            bg=BACKGROUND,
            highlightthickness=PANEL_BORDER,
            highlightbackground=LABEL_FG,
        )
        # The panel keeps its requested width instead of shrinking around its
        # content. Its height follows the board through fill=tk.Y.
        self.frame.pack_propagate(False)

        # ----- turn indicator -----
        # Both tiles draw the king glyph and are told apart by color. The
        # white king sits on the dark square color and the black king on the
        # light one, so either glyph stays readable.
        indicator = tk.Frame(self.frame, bg=BACKGROUND)
        indicator.pack(side=tk.TOP, fill=tk.X, padx=6, pady=6)

        kingRow = tk.Frame(indicator, bg=BACKGROUND)
        kingRow.pack(side=tk.TOP, fill=tk.X)
        self.whiteKingLabel = tk.Label(
            kingRow, bg=DARK, fg="white", font=KING_FONT, padx=10, pady=2,
            highlightthickness=3, highlightbackground=BACKGROUND,
        )
        self.whiteKingLabel.pack(side=tk.LEFT, expand=True)
        self.blackKingLabel = tk.Label(
            kingRow, bg=LIGHT, fg="black", font=KING_FONT, padx=10, pady=2,
            highlightthickness=3, highlightbackground=BACKGROUND,
        )
        self.blackKingLabel.pack(side=tk.RIGHT, expand=True)

        self.turnLabel = tk.Label(indicator, bg=BACKGROUND, fg=LABEL_FG, font=TURN_FONT)
        self.turnLabel.pack(side=tk.TOP, pady=(4, 0))

        # ----- move history -----
        # One vertical scrollbar moves all three columns together. The
        # columns do not wrap. A wrapped line could stop the rows lining up
        # with their move numbers. place() pins the history to the bottom
        # part of the panel and keeps that share when the window resizes.
        self.movesFrame = tk.Frame(self.frame, bg=BACKGROUND)
        self.movesFrame.place(relx=0.0, rely=0.4, relwidth=1.0, relheight=0.6)

        # The header row, the columns and the copy button share one grid.
        # This lines the headers up with the columns they name, and the two
        # expanding move columns keep the labels centered above them at any
        # panel width. Columns: number, rule, white, rule, black, scrollbar.
        # Rows: header, top rule, scrolling columns, bottom rule, copy
        # button.
        for col in (2, 4):
            self.movesFrame.columnconfigure(col, weight=1)
        self.movesFrame.rowconfigure(2, weight=1)

        tk.Label(
            self.movesFrame, text="White", bg=BACKGROUND, fg=DARK,
            font=MOVE_FONT, pady=1,
        ).grid(row=0, column=2)
        tk.Label(
            self.movesFrame, text="Black", bg=BACKGROUND, fg=DARK,
            font=MOVE_FONT, pady=1,
        ).grid(row=0, column=4)
        tk.Frame(self.movesFrame, height=1, bg=DARK).grid(
            row=1, column=0, columnspan=6, sticky="ew")
        # A selection cannot span the separate columns, so this button
        # copies the whole history (move numbers and both players' moves)
        # to the clipboard as one text.
        self.copyButton = tk.Button(
            self.movesFrame, text="Copy Move History", command=self.copyHistory,
            relief=tk.FLAT, bd=0, pady=1, bg=BACKGROUND,
            activebackground=LIGHT, fg=LABEL_FG,
            takefocus=0, cursor="hand2",
        )
        # Bottom rule, under the move columns.
        tk.Frame(self.movesFrame, height=1, bg=DARK).grid(
            row=3, column=0, columnspan=6, sticky="ew")
        self.copyButton.grid(row=4, column=0, columnspan=6, sticky="ew")

        # Flag to stop a scroll looping back through the shared scroll
        # callback when one of the widgets is scrolled.
        self.isSyncingScroll = False

        self.movesScroll = ttk.Scrollbar(
            self.movesFrame, orient=tk.VERTICAL, command=self.scrollMoves)
        self.movesScroll.grid(row=2, column=5, sticky="ns")

        # The columns are flat Text widgets, borderless and in the panel's
        # own colors. They are Text widgets because only Text can be
        # scrolled; a Label cannot.
        def rule(column):
            tk.Frame(self.movesFrame, width=1, bg=DARK).grid(
                row=2, column=column, sticky="ns")

        def column(width, padx, fg, gridCol, expand):
            box = tk.Text(
                self.movesFrame, wrap=tk.NONE, width=width, height=1,
                relief=tk.FLAT, bd=0, highlightthickness=0,
                padx=padx, pady=2, font=MOVE_FONT,
                state=tk.DISABLED, takefocus=0, cursor="arrow",
                bg=BACKGROUND, fg=fg,
            )
            box.grid(row=2, column=gridCol, sticky="ns" if not expand else "nsew")
            return box

        # The move columns share the width left over. The number column only
        # needs its three characters.
        self.moveNumbers = column(3, 2, DARK, 0, False)
        self.moveNumbers.tag_configure("num", justify=tk.RIGHT)
        rule(1)
        self.whiteMoves = column(1, 4, LABEL_FG, 2, True)
        self.whiteMoves.tag_configure("line", justify=tk.LEFT)
        rule(3)
        self.blackMoves = column(1, 4, LABEL_FG, 4, True)
        self.blackMoves.tag_configure("line", justify=tk.LEFT)

        # All three columns scroll as one unit. Each reports through the
        # same scroll callback, and the mouse wheel scrolls them together.
        self.moveWidgets = (self.moveNumbers, self.whiteMoves, self.blackMoves)
        for widget in self.moveWidgets:
            widget.config(yscrollcommand=self.syncScroll)
            widget.bind("<MouseWheel>", self.onMovesWheel)
            widget.bind("<Button-4>", self.onMovesWheel)
            widget.bind("<Button-5>", self.onMovesWheel)

        # A game starts with white to move. The backend calls setTurn() from then on.
        self.setTurn("white")

    def postEvent(self, event):
        """
        Queues an event for the C++ backend through the callable given at
        construction. Panel buttons should send their events through here.

        @param event The event string, in the space-separated format the
               C++ side parses, e.g., "save_board".
        """
        self.addEvent(event)

    def updateGlyphs(self):
        """
        Re-reads the piece glyphs given at construction. The backend calls
        this whenever it registers a piece, so the panel picks up the king
        glyph even when the glyph arrives after the panel was built.
        """
        # Nothing to draw until the king is registered.
        if "king" not in self.glyphs:
            return
        self.whiteKingLabel.config(text=self.glyphs["king"])
        self.blackKingLabel.config(text=self.glyphs["king"])

    def setTurn(self, color):
        """
        Moves the turn indicator to the player to move. Their king tile is
        highlighted and the label names them.

        @param color "white" or "black".
        """
        color = color.lower()
        if color not in ("white", "black"):
            raise ValueError(f"Unknown color '{color}'; expected white or black")

        self.updateGlyphs()
        white = color == "white"
        self.whiteKingLabel.config(
            highlightbackground=SELECTED_HIGHLIGHT if white else BACKGROUND)
        self.blackKingLabel.config(
            highlightbackground=BACKGROUND if white else SELECTED_HIGHLIGHT)
        self.turnLabel.config(text=("White" if white else "Black") + "'s Turn!")

    def addMove(self, color, move):
        """
        Appends one move to the given player's history box and scrolls it into view.

        @param color "white" or "black", the player who made the move.
        @param move The move text to display, e.g., "e2e4".
        """
        color = color.lower()
        if color not in ("white", "black"):
            raise ValueError(f"Unknown color '{color}'; expected white or black")

        box = self.whiteMoves if color == "white" else self.blackMoves
        box.config(state=tk.NORMAL)
        # Drop the blank pad row if the box has one, so the new move is added
        # on its own line. Moves are never blank, so any empty line is padding.
        while (int(box.index("end-1l").split(".")[0]) > 1
               and box.get("end-2l", "end-1l").strip() == ""):
            box.delete("end-2l", "end-1l")
        box.insert(tk.END, move + "\n", "line")
        box.config(state=tk.DISABLED)
        self.padShorterBox()
        self.refreshMoveNumbers()
        box.see(tk.END)

    def clearMoves(self):
        """
        Empties both players' move history boxes. The backend calls this when
        a new game starts so the boxes only ever hold the current game's moves.
        """
        for box in (self.whiteMoves, self.blackMoves):
            box.config(state=tk.NORMAL)
            box.delete("1.0", tk.END)
            box.config(state=tk.DISABLED)
        self.refreshMoveNumbers()
        self.moveNumbers.yview_moveto(0.0)

    def markLastMove(self, color, marker):
        """
        Appends a marker to the end of the given player's most recent move
        line, e.g., "#" on the move which delivered checkmate. Does nothing
        when that player's box is still empty.

        @param color "white" or "black", the player who made the move.
        @param marker The text to append to the last move.
        """
        color = color.lower()
        if color not in ("white", "black"):
            raise ValueError(f"Unknown color '{color}'; expected white or black")

        box = self.whiteMoves if color == "white" else self.blackMoves
        if box.get("1.0", "end-2c").strip() == "":
            return
        box.config(state=tk.NORMAL)
        # "end-1c" is the implicit newline tkinter keeps at the end of every
        # Text, so "end-2c" is the position right after the last move's text.
        box.insert("end-2c", marker)
        box.see(tk.END)
        box.config(state=tk.DISABLED)

    def removeLastMove(self, color):
        """
        Removes the most recent move from the given player's history box, the
        inverse of addMove(). Does nothing when that player's box is still empty.

        @param color "white" or "black", the player who made the last move.
        """
        color = color.lower()
        if color not in ("white", "black"):
            raise ValueError(f"Unknown color '{color}'; expected white or black")

        box = self.whiteMoves if color == "white" else self.blackMoves
        if box.get("1.0", "end-2c").strip() == "":
            return
        box.config(state=tk.NORMAL)
        # Drop any trailing pad lines first (mirrors the pad handling in
        # addMove), then delete the last move line that is left.
        while (int(box.index("end-1l").split(".")[0]) > 1
               and box.get("end-2l", "end-1l").strip() == ""):
            box.delete("end-2l", "end-1l")
        if int(box.index("end-1l").split(".")[0]) > 1:
            box.delete("end-2l", "end-1l")
        box.config(state=tk.DISABLED)
        self.padShorterBox()
        self.refreshMoveNumbers()
        self.moveNumbers.yview_moveto(0.0)

    def copyHistory(self):
        """
        Copies the whole history to the clipboard as one text, one row per
        move number (e.g., "1. w: e2 -> e4  b: e7 -> e5"). A selection
        cannot span the separate history boxes, so the copy button calls
        this instead.
        """
        whiteMoves = [m for m in self.whiteMoves.get("1.0", "end-2c").splitlines() if m]
        blackMoves = [m for m in self.blackMoves.get("1.0", "end-2c").splitlines() if m]

        rows = []
        for row in range(max(len(whiteMoves), len(blackMoves))):
            line = f"{row + 1}."
            if row < len(whiteMoves):
                line += " " + whiteMoves[row]
            if row < len(blackMoves):
                line += "  " + blackMoves[row]
            rows.append(line)

        if rows:
            self.frame.clipboard_clear()
            self.frame.clipboard_append("\n".join(rows))

    def padShorterBox(self):
        """
        Pads the shorter history box with blank lines so both boxes hold the
        same number of lines. Scrolling moves every column to the same
        fraction of its content, which only lines the rows up with the move
        numbers while the boxes have equally many lines. The pad row also
        marks the move number a player has yet to answer.
        """
        whiteLines = self.countMoveLines(self.whiteMoves)
        blackLines = self.countMoveLines(self.blackMoves)
        box = self.whiteMoves if whiteLines < blackLines else self.blackMoves
        for _ in range(abs(whiteLines - blackLines)):
            box.config(state=tk.NORMAL)
            box.insert(tk.END, "\n", "line")
            box.config(state=tk.DISABLED)

    def refreshMoveNumbers(self):
        """
        Rebuilds the move number column. Row k of the boxes holds each
        player's k-th move, so the column shows one number per row, as far
        as whichever box has more lines.
        """
        rows = max(self.countMoveLines(self.whiteMoves),
                   self.countMoveLines(self.blackMoves))
        self.moveNumbers.config(state=tk.NORMAL)
        self.moveNumbers.delete("1.0", tk.END)
        for row in range(1, rows + 1):
            self.moveNumbers.insert(tk.END, f"{row}\n", "num")
        self.moveNumbers.config(state=tk.DISABLED)

    @staticmethod
    def countMoveLines(box):
        """
        Counts the move lines in one history box. "end-1c" is the implicit
        final line tkinter keeps after a Text's last newline.

        @param box The history Text widget to count the lines of.
        @return The number of move lines.
        """
        return int(box.index("end-1c").split(".")[0]) - 1

    def scrollMoves(self, *args):
        """
        Scrollbar command: scrolls all the history columns together.

        @param args The arguments the scrollbar passes through to yview.
        """
        for widget in self.moveWidgets:
            widget.yview(*args)

    def syncScroll(self, first, last):
        """
        yscrollcommand for every history column. It copies the scroll
        position to the other columns and the scrollbar so they always line
        up. The guard flag stops the scrolled widgets from calling back.

        @param first Fraction of the content scrolled above the visible area.
        @param last Fraction of the content up to the bottom of the visible
            area.
        """
        if self.isSyncingScroll:
            return
        self.isSyncingScroll = True
        self.movesScroll.set(first, last)
        for widget in self.moveWidgets:
            widget.yview_moveto(first)
        self.isSyncingScroll = False

    def onMovesWheel(self, event):
        """
        Mouse wheel handler for the history columns. Scrolls all of them
        together, whichever one the wheel is over.

        @param event The wheel event, a wheel button on X11 or a MouseWheel
            event elsewhere.
        @return "break" so the wheel does not also scroll anything above.
        """
        num = getattr(event, "num", 0)
        if num == 4:
            direction = -1
        elif num == 5:
            direction = 1
        elif getattr(event, "delta", 0) > 0:
            direction = -1
        else:
            direction = 1
        for widget in self.moveWidgets:
            widget.yview_scroll(direction, "units")
        return "break"

    def show(self):
        """
        Opens the panel to the right of the collapse bar, which widens the
        window. Does nothing when the panel is already open.
        """
        if self.visible:
            return
        self.frame.pack(side=tk.LEFT, fill=tk.Y)
        self.bar.config(text=BAR_OPEN)
        self.visible = True

    def hide(self):
        """
        Closes the panel, leaving only the collapse bar. Does nothing when
        the panel is already closed.
        """
        if not self.visible:
            return
        self.frame.pack_forget()
        self.bar.config(text=BAR_COLLAPSED)
        self.visible = False

    def toggle(self):
        """
        Opens the panel when it is closed, and closes it when it is open.
        The collapse bar calls this when clicked.
        """
        if self.visible:
            self.hide()
        else:
            self.show()

    def isVisible(self):
        """
        @return True when the panel is currently open beside the board.
        """
        return self.visible

##################
# STANDALONE DEMO
##################

def main():
    """
    Standalone demo: a dummy board next to the panel, with no C++ backend
    and no checkUI import. Events are printed to the console instead of
    being queued.
    """
    root = tk.Tk()
    root.title("Options panel demo")

    row = tk.Frame(root)
    row.pack()

    # Dummy board standing in for the ChessBoard widget.
    board = tk.Frame(row, width=5 * 90 + 2 * 34, height=6 * 90 + 2 * 34, bg="#7d6455")
    board.pack(side=tk.LEFT)
    board.pack_propagate(False)

    panel = OptionsPanel(row, print, {"king": "\u265A"})
    panel.addMove("white", "e2e4")
    panel.addMove("black", "e7e5")
    panel.addMove("white", "g1f3")
    panel.setTurn("black")
    panel.show()
    root.mainloop()

if __name__ == "__main__":
    main()
