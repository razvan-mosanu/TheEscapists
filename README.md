# The Escapists - C++

A 2D top-down strategy and simulation game inspired by *The Escapists*. You play as an inmate, and your ultimate goal is to strategize, craft, and fight your way out of prison. The game is built from scratch using modern C++ and the SFML graphics library.

## Features

* **Prison Routine:** Survive by following the daily prison schedule (Morning Rollcall, Breakfast, Free Time, Work, Evening Rollcall, Lights Out). Skipping routines will increase your "Heat" and make guards suspicious!
* **Dynamic NPC AI:** 
  * **Guards:** Patrol the prison, search suspicious inmates for contraband, and engage in combat if provoked.
  * **Inmates:** Wander around, attend routines, and offer black-market items for trade.
  * **The Warden:** A unique boss-like entity wandering the prison.
* **Crafting System:** Collect or buy basic items (Iron Ingots, Sticks, Duct Tape, Rope) and combine them to craft essential tools like a **Pickaxe**, **Grapple Hook**, or **Shiv**.
* **Combat System:** Toggle combat mode and punch your way out of trouble. Knock out guards for a chance to steal the coveted Warden Key!
* **Stats & Training:** Hit the gym during your free time to spend energy (stamina) on increasing your Strength, Speed, and IQ.
* **Environment Interaction:** Use your crafted tools to break down prison walls or access restricted areas (requires the Warden Key).
* **Inventory Management:** A fully interactive drag-and-drop inventory system and a personal stash hidden in your cell to store illegal contraband.

## Controls

| Key / Action | Description |
| :--- | :--- |
| **W, A, S, D** | Move your character |
| **Space** | Toggle Combat Mode ON / OFF |
| **B** | Punch (You must be in Combat Mode) |
| **E** | Interact / Trade (near inmates) / Access Stash |
| **F** | Break Wall (Requires Pickaxe) |
| **T** | Train Stats (You must be in the Gym) |
| **I** | Open Full Inventory |
| **Q** | View Character Stats |
| **C** | Open Crafting Menu |
| **Left Click** | Drag & Drop items in your inventory |
| **Enter/Escape** | Navigate Menus / Close UI |

## Technical Details

This project was built focusing on clean Object-Oriented Programming (OOP) principles:
- **Language:** C++23
- **Graphics:** [SFML 2.6.x](https://www.sfml-dev.org/)
- **Map Parsing:** [nlohmann/json](https://github.com/nlohmann/json) for parsing `.tmj` maps exported from [Tiled](https://www.mapeditor.org/).
- **Design Patterns Used:**
  - *Singleton* (CRTP based) for centralized managers (`RandomGenerator`, `ItemFactory`).
  - *Factory* for spawning various game items.
  - *Observer* for a robust event system decoupling entity deaths from game logic.
- **Advanced C++ Features:** Extensive use of `std::shared_ptr`, smart pointers, templates, `<random>`, and standard algorithms.

## License

The project is licensed under [AGPLv3](LICENSE).
The base OOP [template repository](https://github.com/mcmarius/oop-template) is licensed under [Unlicense](LICENSE.template).

## Resources & Credits
- JSON Parser: [nlohmann/json](https://github.com/nlohmann/json)
- Graphics Library: [SFML Documentation](https://www.sfml-dev.org/documentation/3.1.0/)
- Map Editor: [Tiled](https://www.mapeditor.org/)
- Original game sprites & inspiration: [The Escapists Sprites](https://www.spriters-resource.com/pc_computer/theescapists/)
