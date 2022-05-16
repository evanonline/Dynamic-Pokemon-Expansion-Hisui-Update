#include "defines.h"
#include "../include/evolution.h"
#include "../include/graphics.h"
#include "../include/main.h"
#include "../include/pokedex.h"

//Backsprite battle start

extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u16 gSpeciesIdToCryId[];
extern const u16 gSpeciesToNationalPokedexNum[];

extern const u16 gPokedexOrder_Regional[];
extern const u16 gRegionalDexCount;
extern const u16 gPokedexOrder_Alphabetical[];
extern const u16 gPokedexOrderAlphabeticalCount;
extern const u16 gPokedexOrder_Weight[];
extern const u16 gPokedexOrderWeightCount;
extern const u16 gPokedexOrder_Height[];
extern const u16 gPokedexOrderHeightCount;
extern const u16 gPokedexOrder_Type[];
extern const u16 gPokedexOrderTypeCount;

extern const struct AlternateDexEntries gAlternateDexEntries[];
extern const struct CompressedSpriteSheet gMonBackPicTable[];
extern const struct CompressedSpriteSheet gMonFrontPicTable[];
extern const struct CompressedSpritePalette gMonPaletteTable[];
extern const struct CompressedSpritePalette gMonShinyPaletteTable[];

const u16 gNumSpecies = NUM_SPECIES;
const u16 gNumDexEntries = FINAL_DEX_ENTRY;

u8 __attribute__((long_call)) GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);
u8  __attribute__((long_call)) GetUnownLetterFromPersonality(u32 personality);
bool8 __attribute__((long_call)) GetSetPokedexFlag(u16 nationalNum, u8 caseID);
s8 __attribute__((long_call)) DexFlagCheck(u16 nationalDexNo, u8 caseId, bool8 indexIsSpecies);
u16 __attribute__((long_call)) SpeciesToNationalPokedexNum(u16 species);
void __attribute__((long_call)) break_func();

//This file's functions
u16 TryGetFemaleGenderedSpecies(u16 species, u32 personality);
static u16 LoadNationalPokedexView(void);

u16 SpeciesToCryId(u16 species)
{
	return species + 1;
}

struct PivotalDexSpecies
{
	u16 species;
	u16 dexNum;
};

static const struct PivotalDexSpecies sPivotalDexSpecies[] =
{
	//These species have Pokemon grouped in order after them
	{SPECIES_ROWLET, NATIONAL_DEX_ROWLET},
	{SPECIES_CHESPIN, NATIONAL_DEX_CHESPIN},
	{SPECIES_TURTWIG, NATIONAL_DEX_TURTWIG},
	{SPECIES_TREECKO, NATIONAL_DEX_TREECKO},
	{SPECIES_BULBASAUR, NATIONAL_DEX_BULBASAUR},
};

u16 NationalPokedexNumToSpecies(u16 nationalNum)
{
	u16 species, i;

	if (nationalNum == 0)
		return 0;

	species = 0;
	
	//Optimization
	if (nationalNum <= SPECIES_SHIFTRY || nationalNum >= SPECIES_TURTWIG) //Hoenn Mons are too out of order for this to work
	{
		for (i = 0; i < ARRAY_COUNT(sPivotalDexSpecies); ++i)
		{
			if (nationalNum > sPivotalDexSpecies[i].dexNum)
			{
				u16 difference = nationalNum - sPivotalDexSpecies[i].dexNum;
				if (gSpeciesToNationalPokedexNum[sPivotalDexSpecies[i].species + difference - 1] == nationalNum)
					return sPivotalDexSpecies[i].species + difference;
				break;
			}
		}
	}

	while (species < (NUM_SPECIES - 1) && gSpeciesToNationalPokedexNum[species] != nationalNum)
		species++;

	if (species == NUM_SPECIES - 1)
		return 0;

	return species + 1;
}

const u8* TryLoadAlternateDexEntry(u16 species)
{
	for (int i = 0; gAlternateDexEntries[i].species != SPECIES_TABLES_TERMIN; ++i)
	{
		if (gAlternateDexEntries[i].species == species)
			return gAlternateDexEntries[i].description;
	}
	
	return 0;
}

void LoadSpecialPokePic(const struct CompressedSpriteSheet* src, void* dest, u16 species, u32 personality, bool8 isFrontPic)
{
	u16 oldSpecies = species;
	const struct CompressedSpriteSheet* table = isFrontPic ? gMonFrontPicTable : gMonBackPicTable;

	species = TryGetFemaleGenderedSpecies(species, personality);
	if (species != oldSpecies) //Updated sprite
		src = &table[species];
	
	if (species == SPECIES_UNOWN)
	{
		u16 i = GetUnownLetterFromPersonality(personality);

		// The other Unowns are separate from Unown A.
		if (i == 0)
			i = SPECIES_UNOWN;
		else
			i += SPECIES_UNOWN_B - 1;

		if (!isFrontPic)
			LZ77UnCompWram((void*) gMonBackPicTable[i].data, dest);
		else
			LZ77UnCompWram((void*) gMonFrontPicTable[i].data, dest);
	}
	else if (species > NUM_SPECIES) // is species unknown? draw the ? icon
		LZ77UnCompWram((void*) gMonFrontPicTable[0].data, dest);
	else
		LZ77UnCompWram((void*) src->data, dest);

	DrawSpindaSpots(species, personality, dest, isFrontPic);
}

const u32* GetFrontSpritePalFromSpeciesAndPersonality(u16 species, u32 otId, u32 personality)
{
	u32 shinyValue;
	
	species = TryGetFemaleGenderedSpecies(species, personality);
	
	if (species > NUM_SPECIES)
		return (u32*) gMonPaletteTable[0].data;

	shinyValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);
	if (shinyValue < 8)
		return (u32*) gMonShinyPaletteTable[species].data;
	else
		return (u32*) gMonPaletteTable[species].data;
}

const struct CompressedSpritePalette* GetMonSpritePalStructFromOtIdPersonality(u16 species, u32 otId , u32 personality)
{
	u32 shinyValue;
	species = TryGetFemaleGenderedSpecies(species, personality);

	shinyValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);
	if (shinyValue < 8)
		return &gMonShinyPaletteTable[species];
	else
		return &gMonPaletteTable[species];
}

u16 TryGetFemaleGenderedSpecies(u16 species, u32 personality)
{
	if (GetGenderFromSpeciesAndPersonality(species, personality) == MON_FEMALE)
	{
		switch (species) {
			case SPECIES_HIPPOPOTAS:
				species = SPECIES_HIPPOPOTAS_F;
				break;
			case SPECIES_HIPPOWDON:
				species = SPECIES_HIPPOWDON_F;
				break;
			case SPECIES_UNFEZANT:
				species = SPECIES_UNFEZANT_F;
				break;
			case SPECIES_FRILLISH:
				species = SPECIES_FRILLISH_F;
				break;
			case SPECIES_JELLICENT:
				species = SPECIES_JELLICENT_F;
				break;
			case SPECIES_PYROAR:
				species = SPECIES_PYROAR_FEMALE;
				break;
			case SPECIES_VENUSAUR:
				species = SPECIES_VENUSAUR_F;
				break;
			case SPECIES_BUTTERFREE:
				species = SPECIES_BUTTERFREE_F;
				break;	
			case SPECIES_RATTATA:
				species = SPECIES_RATTATA_F;
				break;
			case SPECIES_RATICATE:
				species = SPECIES_RATICATE_F;
				break;
			case SPECIES_PIKACHU:
				species = SPECIES_PIKACHU_F;
				break;
			case SPECIES_RAICHU:
				species = SPECIES_RAICHU_F;
				break;
			case SPECIES_ZUBAT:
				species = SPECIES_ZUBAT_F;
				break;
			case SPECIES_GOLBAT:
				species = SPECIES_GOLBAT_F;
				break;
			case SPECIES_GLOOM:
				species = SPECIES_GLOOM_F;
				break;
			case SPECIES_VILEPLUME:
				species = SPECIES_VILEPLUME_F;
				break;
			case SPECIES_KADABRA:
				species = SPECIES_KADABRA_F;
				break;
			case SPECIES_ALAKAZAM:
				species = SPECIES_ALAKAZAM_F;
				break;
			case SPECIES_DODUO:
				species = SPECIES_DODUO_F;
				break;
			case SPECIES_DODRIO:
				species = SPECIES_DODRIO_F;
				break;
			case SPECIES_HYPNO:
				species = SPECIES_HYPNO_F;
				break;
			case SPECIES_RHYHORN:
				species = SPECIES_RHYHORN_F;
				break;
			case SPECIES_RHYDON:
				species = SPECIES_RHYDON_F;
				break;
			case SPECIES_GOLDEEN:
				species = SPECIES_GOLDEEN_F;
				break;
			case SPECIES_SEAKING:
				species = SPECIES_SEAKING_F;
				break;
			case SPECIES_SCYTHER:
				species = SPECIES_SCYTHER_F;
				break;
			case SPECIES_MAGIKARP:
				species = SPECIES_MAGIKARP_F;
				break;
			case SPECIES_GYARADOS:
				species = SPECIES_GYARADOS_F;
				break;		
			case SPECIES_EEVEE:
				species = SPECIES_EEVEE_F;
				break;
			case SPECIES_MEGANIUM:
				species = SPECIES_MEGANIUM_F;
				break;
			case SPECIES_LEDYBA:
				species = SPECIES_LEDYBA_F;
				break;
			case SPECIES_LEDIAN:
				species = SPECIES_LEDIAN_F;
				break;
			case SPECIES_XATU:
				species = SPECIES_XATU_F;
				break;		
			case SPECIES_SUDOWOODO:
				species = SPECIES_SUDOWOODO_F;
				break;
			case SPECIES_POLITOED:
				species = SPECIES_POLITOED_F;
				break;
			case SPECIES_AIPOM:
				species = SPECIES_AIPOM_F;
				break;
			case SPECIES_WOOPER:
				species = SPECIES_WOOPER_F;
				break;						
			case SPECIES_QUAGSIRE:
				species = SPECIES_QUAGSIRE_F;
				break;							
			case SPECIES_MURKROW:
				species = SPECIES_MURKROW_F;
				break;
			case SPECIES_WOBBUFFET:
				species = SPECIES_WOBBUFFET_F;
				break;
			case SPECIES_GIRAFARIG:
				species = SPECIES_GIRAFARIG_F;
				break;							
			case SPECIES_GLIGAR:
				species = SPECIES_GLIGAR_F;
				break;			
			case SPECIES_STEELIX:
				species = SPECIES_STEELIX_F;
				break;			
			case SPECIES_SCIZOR:
				species = SPECIES_SCIZOR_F;
				break;			
			case SPECIES_HERACROSS:
				species = SPECIES_HERACROSS_F;
				break;			
			case SPECIES_SNEASEL:
				species = SPECIES_SNEASEL_F;
				break;
			case SPECIES_URSARING:
				species = SPECIES_URSARING_F;
				break;			
			case SPECIES_PILOSWINE:
				species = SPECIES_PILOSWINE_F;
				break;			
			case SPECIES_OCTILLERY:
				species = SPECIES_OCTILLERY_F;
				break;	
			case SPECIES_HOUNDOOM:
				species = SPECIES_HOUNDOOM_F;
				break;						
			case SPECIES_DONPHAN:
				species = SPECIES_DONPHAN_F;
				break;		
			case SPECIES_TORCHIC:
				species = SPECIES_TORCHIC_F;
				break;			
			case SPECIES_COMBUSKEN:
				species = SPECIES_COMBUSKEN_F;
				break;	
			case SPECIES_BLAZIKEN:
				species = SPECIES_BLAZIKEN_F;
				break;						
			case SPECIES_BEAUTIFLY:
				species = SPECIES_BEAUTIFLY_F;
				break;						
			case SPECIES_DUSTOX:
				species = SPECIES_DUSTOX_F;
				break;						
			case SPECIES_LUDICOLO:
				species = SPECIES_LUDICOLO_F;
				break;						
			case SPECIES_NUZLEAF:
				species = SPECIES_NUZLEAF_F;
				break;						
			case SPECIES_SHIFTRY:
				species = SPECIES_SHIFTRY_F;
				break;			
			case SPECIES_MILOTIC:
				species = SPECIES_MILOTIC_F;
				break;		
			case SPECIES_NUMEL:
				species = SPECIES_NUMEL_F;
				break;		
			case SPECIES_CAMERUPT:
				species = SPECIES_CAMERUPT_F;
				break;				
			case SPECIES_CACTURNE:
				species = SPECIES_CACTURNE_F;
				break;	
			case SPECIES_MEDITITE:
				species = SPECIES_MEDITITE_F;
				break;						
			case SPECIES_MEDICHAM:
				species = SPECIES_MEDICHAM_F;
				break;
			case SPECIES_ROSELIA:
				species = SPECIES_ROSELIA_F;
				break;		
			case SPECIES_GULPIN:
				species = SPECIES_GULPIN_F;	
				break;					
			case SPECIES_SWALOT:
				species = SPECIES_SWALOT_F;
				break;		
			case SPECIES_RELICANTH:
				species = SPECIES_RELICANTH_F;
				break;		
			case SPECIES_STARLY:
				species = SPECIES_STARLY_F;
				break;			
			case SPECIES_STARAVIA:
				species = SPECIES_STARAVIA_F;
				break;					
			case SPECIES_STARAPTOR:
				species = SPECIES_STARAPTOR_F;
				break;	
			case SPECIES_BIDOOF:
				species = SPECIES_BIDOOF_F;
				break;
			case SPECIES_BIBAREL:
				species = SPECIES_BIBAREL_F;
				break;
			case SPECIES_KRICKETOT:
				species = SPECIES_KRICKETOT_F;
				break;
			case SPECIES_KRICKETUNE:
				species = SPECIES_KRICKETUNE_F;
				break;
			case SPECIES_SHINX:
				species = SPECIES_SHINX_F;
				break;
			case SPECIES_LUXIO:
				species = SPECIES_LUXIO_F;
				break;		
			case SPECIES_LUXRAY:
				species = SPECIES_LUXRAY_F;
				break;		
			case SPECIES_COMBEE:
				species = SPECIES_COMBEE_F;
				break;				
			case SPECIES_PACHIRISU:
				species = SPECIES_PACHIRISU_F;
				break;	
			case SPECIES_BUIZEL:
				species = SPECIES_BUIZEL_F;
				break;						
			case SPECIES_FLOATZEL:
				species = SPECIES_FLOATZEL_F;
				break;
			case SPECIES_AMBIPOM:
				species = SPECIES_AMBIPOM_F;
				break;
			case SPECIES_GIBLE:
				species = SPECIES_GIBLE_F;
				break;
			case SPECIES_GABITE:
				species = SPECIES_GABITE_F;
				break;		
			case SPECIES_GARCHOMP:
				species = SPECIES_GARCHOMP_F;
				break;				
			case SPECIES_CROAGUNK:
				species = SPECIES_CROAGUNK_F;
				break;	
			case SPECIES_TOXICROAK:
				species = SPECIES_TOXICROAK_F;
				break;						
			case SPECIES_FINNEON:
				species = SPECIES_FINNEON_F;
				break;						
			case SPECIES_LUMINEON:
				species = SPECIES_LUMINEON_F;
				break;						
			case SPECIES_SNOVER:
				species = SPECIES_SNOVER_F;
				break;						
			case SPECIES_ABOMASNOW:
				species = SPECIES_ABOMASNOW_F;
				break;	
			case SPECIES_WEAVILE:
				species = SPECIES_WEAVILE_F;
				break;		
			case SPECIES_RHYPERIOR:
				species = SPECIES_RHYPERIOR_F;
				break;					
			case SPECIES_TANGROWTH:
				species = SPECIES_TANGROWTH_F;
				break;						
			case SPECIES_MAMOSWINE:
				species = SPECIES_MAMOSWINE_F;
				break;						
			case SPECIES_SNEASEL_H:
				species = SPECIES_SNEASEL_H_F;
				break;
		}
	}
	else if (species == SPECIES_XERNEAS && !gMain.inBattle)
		species = SPECIES_XERNEAS_NATURAL;
	
	return species;
}

u16 GetIconSpecies(u16 species, u32 personality)
{
	u16 result;

	if (species == SPECIES_UNOWN)
	{
		u16 letter = GetUnownLetterFromPersonality(personality);
		if (letter == 0)
			letter = SPECIES_UNOWN;
		else
			letter += (SPECIES_UNOWN_B - 1);
		result = letter;
	}
	else
	{
		if (species > NUM_SPECIES)
			result = 0;
		else
			result = TryGetFemaleGenderedSpecies(species, personality);
	}

	return result;
}

bool8 IsInBattle(void)
{
	return gMain.inBattle;
}


u16 CountSpeciesInDex(u8 caseId, bool8 whichDex)
{
	u16 count = 0;
	u16 i;

	switch (whichDex) {
		case 0: //Regional
			for (i = 0; i < gRegionalDexCount; ++i)
			{
				if (DexFlagCheck(SpeciesToNationalPokedexNum(gPokedexOrder_Regional[i]), caseId, FALSE))
					count++;
			}
			break;
		case 1: //National
			for (i = 1; i <= FINAL_DEX_ENTRY; ++i)
			{
				if (DexFlagCheck(i, caseId, FALSE))
					count++;
			}
			break;
	}

	return count;
}

u16 GetRegionalPokedexCount(u8 caseId)
{
	u16 i, count;

	for (i = 0, count = 0; i < gRegionalDexCount; ++i)
	{
		if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(gPokedexOrder_Regional[i]), caseId))
			count++;
	}

	return count;
}

bool16 HasAllRegionalMons(void)
{
	u16 i;

	for (i = 0; i < gRegionalDexCount; ++i)
	{
		if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(gPokedexOrder_Regional[i]), FLAG_GET_CAUGHT))
			return FALSE;
	}

	return TRUE;
}

bool16 sp1B9_SeenAllRegionalMons(void)
{
	u16 i;

	for (i = 0; i < gRegionalDexCount; ++i)
	{
		if (!GetSetPokedexFlag(SpeciesToNationalPokedexNum(gPokedexOrder_Regional[i]), FLAG_GET_SEEN))
			return FALSE;
	}

	return TRUE;
}

bool16 HasAllMons(void)
{
	u16 i;

	for (i = 1; i <= FINAL_DEX_ENTRY; ++i)
	{
		if (!GetSetPokedexFlag(i, FLAG_GET_CAUGHT))
			return FALSE;
	}

	return TRUE;
}

u16 SpeciesToRegionalDexNum(u16 species)
{
	u16 i;

	for (i = 0; i < gRegionalDexCount; ++i)
	{
		if (gPokedexOrder_Regional[i] == species)
			return i + 1;
	}
	
	return 0;
}

extern const u16 gPokedexOrder_Regional[];
extern const u16 gRegionalDexCount;
extern const u16 gPokedexOrder_Alphabetical[];
extern const u16 gPokedexOrderAlphabeticalCount;
extern const u16 gPokedexOrder_Weight[];
extern const u16 gPokedexOrderWeightCount;
extern const u16 gPokedexOrder_Height[];
extern const u16 gPokedexOrderHeightCount;
extern const u16 gPokedexOrder_Type[];
extern const u16 gPokedexOrderTypeCount;

u16 LoadPokedexViews(u8 type)
{
	u16 i, counter, count, lastMeaningfulIndex;
	const u16* dexList;
	bool8 showUnseenSpecies = FALSE;
	bool8 showUncaughtSpecies = FALSE;

	switch (type) {
		case 0:
			dexList = gPokedexOrder_Regional;
			count = gRegionalDexCount;
			showUnseenSpecies = TRUE;
			showUncaughtSpecies = TRUE;
			break;
		case 1:
			dexList = gPokedexOrder_Alphabetical;
			count = gPokedexOrderAlphabeticalCount;
			showUncaughtSpecies = TRUE;
			break;
		case 2:
			dexList = gPokedexOrder_Type;
			count = gPokedexOrderTypeCount;
			break;
		case 3:
			dexList = gPokedexOrder_Weight;
			count = gPokedexOrderWeightCount;
			break;
		case 4:
			dexList = gPokedexOrder_Height;
			count = gPokedexOrderHeightCount;
			break;
		case 5:
		default:
			return LoadNationalPokedexView();
	}

	for (i = 0, counter = 0, lastMeaningfulIndex = 0; i < count; ++i)
	{
		u16 species = dexList[i];
		bool8 seen = DexFlagCheck(species, FLAG_GET_SEEN, TRUE);
		bool8 caught = DexFlagCheck(species, FLAG_GET_CAUGHT, TRUE);
		
		if (!seen)
		{
			if (showUnseenSpecies)
			{
				gPokedexScreenDataPtr->listItem[counter].name = (void*) 0x8415F66; //-----
				gPokedexScreenDataPtr->listItem[counter++].id = species | (0 << 16); //Unseen
			}
		}
		else if (caught || showUncaughtSpecies)
		{
			lastMeaningfulIndex = counter + 1;
			gPokedexScreenDataPtr->listItem[counter].name = gSpeciesNames[species];
			
			if (caught)
				gPokedexScreenDataPtr->listItem[counter++].id = species | (3 << 16); //Caught
			else
				gPokedexScreenDataPtr->listItem[counter++].id = species | (1 << 16); //Seen
		}
	}

	if (lastMeaningfulIndex == 0)
	{
		//Fix empty list
		lastMeaningfulIndex = 1;
		gPokedexScreenDataPtr->listItem[0].name = (void*) 0x8415F66; //-----
		gPokedexScreenDataPtr->listItem[0].id = gPokedexOrder_Regional[0] | (0 << 16); //Unseen
	}

	return lastMeaningfulIndex;
}

static u16 LoadNationalPokedexView(void)
{
	u16 i, lastMeaningfulIndex;

	for (i = 1, lastMeaningfulIndex = 0; i < NATIONAL_DEX_COUNT; ++i)
	{
		bool8 seen = DexFlagCheck(i, FLAG_GET_SEEN, FALSE);
		bool8 caught = DexFlagCheck(i, FLAG_GET_CAUGHT, FALSE);
		u16 species = NationalPokedexNumToSpecies(i);
		u16 listIndex = i - 1;

		if (!seen)
		{
			gPokedexScreenDataPtr->listItem[listIndex].name = (void*) 0x8415F66; //-----
			gPokedexScreenDataPtr->listItem[listIndex].id = species | (0 << 16); //Unseen
		}
		else
		{
			lastMeaningfulIndex = i;
			gPokedexScreenDataPtr->listItem[listIndex].name = gSpeciesNames[species];
			
			if (caught)
				gPokedexScreenDataPtr->listItem[listIndex].id = species | (3 << 16); //Caught
			else
				gPokedexScreenDataPtr->listItem[listIndex].id = species | (1 << 16); //Seen
		}
	}

	return lastMeaningfulIndex;
}
