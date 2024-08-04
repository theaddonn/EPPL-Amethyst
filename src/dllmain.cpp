#include "dllmain.hpp"

// Ran when the mod is loaded into the game by AmethystRuntime
ModFunction void Initialize(AmethystContext& ctx)
{
	// Initialize Amethyst mod backend
	Amethyst::InitializeAmethystMod(ctx);

	// This patches both PistonBlockActor::_attachedBlockWalker and
	// PistonBlockActor::_checkAttachedBlocks respectfully. We patch the 8 bit cmp
	// (compare) instruction to compare the given amount of blocks to push with FF
	// (255) instead of 0C (12) as the piston push limit. These patches not only set
	// the piston push limit to 255 blocks, they also entirely remove the limit.
	// This is due to the cmp instruction being limited to 8 bits and the following
	// setbe (set byte if below or equal) instruction, this all means that the
	// number to compare against is limited to 8 bits (255 is the maximum) and now
	// is always is atleast less or even equal to it.
	Amethyst::GetPatchManager().ApplyPatch(SigScan("48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 90 48 81 EC 70 01 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 60 48 8B FA") + 0x15C, (uint8_t)0xFF);
	Amethyst::GetPatchManager().ApplyPatch(SigScan("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 68 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 50 0F") + 0x54B, (uint8_t)0xFF);
}
