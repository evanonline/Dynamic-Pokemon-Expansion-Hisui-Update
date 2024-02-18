#Pokemon Trails and Gen 9 additions

This is the repo branch specific to my own personal romhack: Pokemon Trails. You'll notice stupid gags like Charizard with one horn from the old Ken Sugimori art, or a entries for a smattering of pathetic TCG card moves, or references to Minnesota (where vikings live in the Pokemon world!). There's also a few other odd changes here and there that I've made over vanilla DPE, such as changes to how Pokemon evolve when they have two different regional forms (Mime Jr., Exeggcute etc).

HOWEVER, I've noticed there are no public branches or forks that I'm aware of which feature Gen 9 Pokemon, so I've decided to make my unfinished work on this public with the hopes that CFRU's community can come together to help me finalize this. As such, unlike my Hisui branch, this branch is less "generalized" and will still include stuff specific to my romhack, for the sake of my own personal ease. You can feel free to ignore all of this extraneous stuff I've done, revert it, or use it yourself - but I hope you can understand that it'd be a bit of trouble for me to do it myself right now and my focus is primarily on getting Gen 9 stuff added for the community. It's easier to delete stuff you won't use than to add things, after all.

#Here's what this Github repo DOES have:

	* Complete icon sprites for all Gen 9 Pokemon, including DLC and forms. Many of these are reworked from Ezerart's icons to fit the Gen 3 palette limitations. https://www.deviantart.com/ezerart/art/Pokemon-Gen-9-Icon-sprites-3DS-Style-944211258 I tried to add animation frames for many of the icons. (There may still be a few that don't have proper second frames and just copy the first frame)
	* Front and Back Sprites for MOST Gen 9 Pokemon - I omitted a few due to a lack of availability for 64x64 sprites that meet my own personal standards. I've included a few touched up sprites of my own - the Scream Tail sprites are my own work and have been publicly available for a year, but I made new front sprites for Quaxly and Toedscool myself. Most of these sprites are taken from pokeemerald-expansion, with a few from Pokecommunity or by King-of-the-x-Roads on DeviantArt. https://www.deviantart.com/kingofthe-x-roads/art/Gen-9-Sprites-Pokemon-Scarlet-and-Violet-908341834
	* I've also taken the liberty of adding sprites and entries for an near-identical Rockruff form that can have Own Tempo and thus evolve into Dusk Lycanroc, with the intention of reflecting how Scarlet & Violet handles Own Tempo Rockruff.
	* include/items.h updated with evo items. (A few are fabricated, like Gimmighoul's, and this extends past the Leon's Base "free spaces" for items - I'm not sure if that might cause problems, but we'll cross that bridge when we get to it.)
	* include/moves.h updated with all Gen 9 moves.
	* include/pokedex.h has defines for all Gen 9 Pokemon.
	* include/species.h has all Gen 9 Pokemon and forms.
	* Base_Stats.c (with some double-checking needed).

#Here's what this repo still needs:

	* Sprites for these Pokemon:
		* Iron Bundle
		* Iron Hands
		* Iron Moth
		* Miraidon
		* Poltchageist (Unremarkable & Masterpiece)
		* Sinistcha (Unremarkable & Masterpiece)
		* Ogerpon (All forms)
		* Gouging Fire
		* Terapagos (All forms)
		* Pecharunt

	* All new Pokemon added to TM and Tutor files.
	* Compression and improvement to cries.
	* Elevation table data.
	* Probably more??

#ANY help with the data entry work needed here would be greatly appreciated. I intend to finish all of this and share it - but the more people who can chip in some of the boring stuff, the sooner we can get this branch in a compiling state!
