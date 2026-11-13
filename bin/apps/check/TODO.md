# TODO

This is a collection of TODO's still open for this application.

## AI to play against

Some sort of automated opponents.

## Docs

Docs for this app need updated.

## Custom board and pieces

The ability to have images and a custom skin is needed for all elements.

## UI Arrows Improvements

The arrows should be made translucent and a bit larger.
Perhaps even add in knight-shaped arrows which form an L shape.

## Unit tests

Unit tests for all these classes need added.

## Save/Loading options

Add the ability to save and load games.
Save and load are a bit tricky; Although saving and loading the board state is fairly straight forward, the initial board configuration (e.g., size, shape, pieces, etc) is determined by the configuration file, and may not match what the save state stored. Therefore, the configuration file for a save state will also be needed during save/load.

## Game clocks

The board state docs mention keeping times and the timing library is linked in for
it, but nothing keeps time yet.

## Add sounds

A sound for putting the opponent into check needs added.

## Verbose and debug output.

The logging and verbose output needs tidied up and improved.
Enhance debug/verbose statements to use board coordinates.

## Config validation

The config files need some validation and the options to have any config field be optional. 
Also, expanding config to change colors and other things in the UI. 
Make sound configuratble too

## Move UI Improvements

Right now, the UI releasing a piece returns it to its previous square, where the backend will update the position if it's a legal move. This results in some choppy UI piece handling and can be improved by re-ordering the events so that the backend controls whether or not a piece should 'snap' back or just go straight to being moved.
