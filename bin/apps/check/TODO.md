# TODO

This is a collection of TODO's still open for this application.

## Castle handling

Add the ability to castle. This also needs generalized for variable board sizes.

## Custom pieces

Custom pieces need added.

## A turn indicator

Some sort of indicator for who's turn it is in the UI is needed.

## AI to play against

Some sort of automated opponents.

## Docs

Docs for this app need made.

## Code cleanup

There's a lot of duplicate code, or code that can be moved to methods.

## Custom board and pieces

The ability to have images and a custom skin is needed for all elements.

## UI Arrows Improvements

The arrows should be made translucent and a bit larger. Perhaps even add in knight-shaped arrows which form an L shape.

## Unit tests

Unit tests for all these classes need added.

## Save/Loading options

Add the ability to save and load games.

## Draw conditions.

Other draw conditions such as threefold repetition need added.

## [bug] Promotion does not lock the board

The game over state is never set for a promotion, so a mate or stalemate delivered
by a promotion leaves the board playable.

## [bug] No board bounds checking

Coordinates coming from both the config and the UI events are used to index the
squares without being checked against the board size first.

## [bug] Config errors are swallowed

A failed config load is logged and then ignored, which lets the board initialize
with sizes that were never set.

## Install the config and UI script

The config and the UI script are not installed with the other resources, so an
installed build only runs out of the build tree.

## Game clocks

The board state docs mention keeping times and the timing library is linked in for
it, but nothing keeps time yet.

## Move history in the UI

The moves are recorded but never shown, some sort of move list is needed.

## Remove the dev warning

The startup warning about the app being in dev needs removed once the app is done.

## Add sounds

The MIA audio libraries are finished and usable, so sound effects should be added to the game.
