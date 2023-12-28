## This is Pokemon Trails but also it's Gen 9 additions.
This is the repo branch specific to my own personal romhack: Pokemon Trails. You'll notice stupid gags like Charizard with one horn from the old Ken Sugimori art, or a entries for a smattering of pathetic TCG card moves, or references to Minnesota (where vikings live in the Pokemon world!). There's also a few other odd changes here and there that I've made over vanilla DPE, such as changes to how Pokemon evolve when they have two different regional forms (Mime Jr., Exeggcute etc).

HOWEVER, I've noticed there are no public branches or forks that I'm aware of which feature Gen 9 Pokemon, so I've decided to make my unfinished work on this public with the hopes that CFRU's community can come together to help me finalize this. As such, unlike my Hisui branch, this branch is less "generalized" and will still include stuff specific to my romhack, for the sake of my own personal ease. You can feel free to ignore all of this extraneous stuff I've done, revert it, or use it yourself - but I hope you can understand that it'd be a bit of trouble for me to do it myself right now and my focus is primarily on getting Gen 9 stuff added for the community. It's easier to delete stuff you won't use than to add things, after all.

## Here's what this Github repo DOES have:

* Complete icon sprites for all Gen 9 Pokemon, including DLC and forms. Many of these are reworked from Ezerart's icons to fit the Gen 3 palette limitations. https://www.deviantart.com/ezerart/art/Pokemon-Gen-9-Icon-sprites-3DS-Style-944211258 I tried to add animation frames for many of the icons. (There may still be a few that don't have proper second frames and just copy the first frame)
* Front and Back Sprites for MOST Gen 9 Pokemon - I omitted a few due to a lack of availability for 64x64 sprites that meet my own personal standards. I've included a few touched up sprites of my own - the Scream Tail sprites are my own work and have been publicly available for a year, but I made new front sprites for Quaxly and Toedscool myself. Most of these sprites are taken from pokeemerald-expansion, with a few from Pokecommunity or by King-of-the-x-Roads on DeviantArt. https://www.deviantart.com/kingofthe-x-roads/art/Gen-9-Sprites-Pokemon-Scarlet-and-Violet-908341834
* I've also taken the liberty of adding sprites and entries for an near-identical Rockruff form that can have Own Tempo and thus evolve into Dusk Lycanroc, with the intention of reflecting how Scarlet & Violet handles Own Tempo Rockruff.
* include/items.h updated with evo items. (A few are fabricated, like Gimmighoul's, and this extends past the Leon's Base "free spaces" for items - I'm not sure if that might cause problems, but we'll cross that bridge when we get to it.)
* include/moves.h updated with all Gen 9 moves.
* include/pokedex.h has defines for all Gen 9 Pokemon.
* include/species.h has all Gen 9 Pokemon and forms.
* src/Species_To_Pokdex_Table.c is done up to Miraidon, but the order in the file is wildly out of order based on an an early internal Pokemon listing from Scarlet & Violet.
* strings/Pokemon_Name_Table.string has all Pokemon up to Miraidon, but again, they're out of order following the Sc/Vi internal listing.
* strings/Pokedex_Data.string has entries up to Miraidon, but yet again, these are out of order, and may possibly need a second pass.

## Here's what this repo still needs:

* Sprites for these Pokemon:
	* Iron Bundle
	* Iron Hands
	* Iron Jugulis
	* Iron Moth
	* Roaring Moon
	* Iron Valiant
	* Miraidon
	* Poltchageist (Unremarkable & Masterpiece)
	* Sinistcha (Unremarkable & Masterpiece)
	* Okidogi
	* Munkidori
	* Ogerpon (All forms)
	* Archaludon (Back sprite only, currently using a placeholder)
	* Hydrapple
	* Gouging Fire
	* Raging Bolt
	* Iron Boulder
	* Iron Crown
	* Terapagos (All forms)
	* Pecharunt
* Reorganization of work I did last year based on early datamined content and internal Pokemon ordering, as mentioned above.
* include/abilities.h needs the new abilities. I'm still working off of the older, pre-dev branch abilities structure; this will need to change in order to accomodate the Gen 9 abilities!
* All new Pokemon added to TM and Tutor files.
* Base stats for all Pokemon except the Sprigatito line.
* Evolution methods. (Likely the next thing I will work on)
* I've included a folder with all Gen 9 cries except DLC Pokemon; these need to be renamed, placed into the main audio folder, and added to the Cry Table. The cries themselves also likely need some condensing. (The Enamorus cry I've been using is also... huge. Need to fix that probably.)
* Egg moves.
* Elevation table data.
* Evolution table data. (Will likely be what I work on next.)
* Footprints. (Usually this is just a copypasting-blank-data job, but it still needs to be done.)
* Front & Back Pic tables and their coord tables.
* Habitat table.
* Icon Palette table & Icon table.
* Learnsets.
* Palette table & Shiny palette table.
* Pokedex data table.
* Pokedex order; I have a regional table that includes all Gen 9 Pokemon up to the DLC, and apparently at some point I started the weight table? But I did not finish it and I don't know where I left off, and the Pokemon who are done in that table may be wildly out of order, because I was most likely going off of an early internal Pokemon listing from Scarlet & Violet at the time when I did this.
* Probably more??

## ANY help with the data entry work needed here would be greatly appreciated. I intend to finish all of this and share it - but the more people who can chip in some of the boring stuff, the sooner we can get this branch in a compiling state!
