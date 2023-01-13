# Shark Bait
First person, underwater survival game.
The player must survive increasingly difficult waves of enemies in the forms of sharks, whilst managing their health, ammo and oxygen levels.
Health and Ammo can be collected through pick-ups scattered across the game environment and oxygen can be collected from bubbles emitted by coral or breaching the water's surface.

This game was created using a given OpenGL based game engine by my university, which I then modified and created all the game elements. This project was created as a final year module project scoring 84%.
### NPCs
#### Hostile
- Sharks
  - State machine based enemy
  - Passive mode: Roams around the gameworld
  - Attack mode: Begins pursuing the player and increases speed
  - Strike mode: Gains further speed boost and attempts to bite the player
  - Flee mode: After a succesful attack the shark swims away from the player
- Jellyfish
  - Constant roaming enemy which damages the player when collided with
#### Passive
- Turtles
  - Roaming passive NPC which when killed grants the player a shield buff
- Dolphins
  - Roaming NPC
  - Grants speed boost to player in a proximity
  - Has a flocking mechanism where dolphins group to other nearby dolphins and move together

| Asset | Source | License |
|-------|--------|---------|
|Shark model/anim/texture	| https://www.cgtrader.com/free-3d-models/animals/fish/realistic-animated-great-white-shark |	Royalty Free License |
| Waterbox Texture |	https://opengameart.org/content/ocean-hdriskybox | Public Domain |
| Harpoon gun and Harpoon model/texture | https://sketchfab.com/3d-models/harpoon-rifle-5456adeff51947d5bf66a4563bb3821f	| CC Attribution |
| Bubble Emitter model/texture | https://www.cgtrader.com/3d-models/plant/other/corals | Royalty Free License|
| Height map | Generated myself using: https://tangrams.github.io/heightmapper/#7.20833/26.401/-59.591 | Public Domain
| Ammo crate texture | https://3dtextures.me/2021/02/12/wood-crate-001/	| Public Domain |
| Sand Texture | https://open3dmodel.com/3d-models/coral-reef-scene_595273.html |	Free for personal use |
| Red background for health pack and crossfade | https://www.needpix.com/photo/1384018/square-red-background | Public Domain |
| Jellyfish model | https://creazilla.com/nodes/3281-set-of-jellyfish-3d-model | Creazilla Open-Source License |
| Turtle model and texture | https://free3d.com/3d-model/-sea-turtle-v1--427786.html | Personal Use License |
| Menu Background	| https://unsplash.com/photos/XexawgzYOBc	| Free to use |
| Boat model and Textures |	https://www.turbosquid.com/3d-models/boat-pbr-model-1522670	| Turbosquid 3D Model License |
| Dolphin model/Texture/Anims |	https://www.cgtrader.com/free-3d-models/animals/mammal/realistic-animated-dolphin |	Royalty Free License |
| Rock Texture | https://www.freepik.com/free-photo/cut-tile-shade-durable-wallpaper_1066914.htm#query=rock%20texture&position=4&from_view=keyword | Free license |
| Game background audio |	https://pixabay.com/music/build-up-scenes-talaso-extasis-119659/ | Simplified Pixabay License |
| Bubbling Audio | https://pixabay.com/sound-effects/bubbling-6184/	| Simplified Pixabay License |
| Shark Bite | https://pixabay.com/sound-effects/monster-bite-44538/	| Simplified Pixabay License |
| Gun Shot | https://pixabay.com/sound-effects/gunshot-37055/ | Simplified Pixabay License |
| Menu Music | https://pixabay.com/music/ambient-dark-cinematic-suspenseful-ambient-111682/ | Simplified Pixabay License
