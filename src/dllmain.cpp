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
	Amethyst::GetPatchManager().ApplyPatch(SlideAddress(0x32D38EC), (uint8_t)0xFF);
	Amethyst::GetPatchManager().ApplyPatch(SlideAddress(0x32D409B), (uint8_t)0xFF);
}
