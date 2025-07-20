# RPG -> Data -> Stats

This folder contains all of the stat related classes for this rpg system.

---

## Structure
- **vital**: The `vital` folder contains all code related to vitals. This represents a value which has some maximum and minimum and ranges as some value between the two.

- **attribute**: The `attribute` folder contains all code related to attributes. This represents some value which represents a single value which can adjust other values (such as vitals).

- **stats.json**: A JSON configuration file defining example vital resources with associated metadata and starting values. Used for initializing or testing stat systems with predefined resource types such as Health, Mana, Strength, Dexterity, Sanity, etc.
