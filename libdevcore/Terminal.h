#pragma once

namespace dev
{
namespace con
{

#if defined(_WIN32)

#define aecReset ""       // Text Reset

#define aecReset ""       // Text Reset

	// Regular Colors
#define aecBlack	""    // Black
#define aecCoal		""    // Black
#define aecGray		""    // White
#define aecWhite	""    // White
#define aecMaroon	""    // Red
#define aecRed		""    // Red
#define aecGreen	""    // Green
#define aecLime		""    // Green
#define aecOrange	""    // Yellow
#define aecYellow	""    // Yellow
#define aecNavy		""    // Blue
#define aecBlue		""    // Blue
#define aecViolet	""    // Purple
#define aecPurple	""    // Purple
#define aecTeal		""    // Cyan
#define aecCyan		""    // Cyan

#define aecBlackBold	""      // Black
#define aecCoalBold		""      // Black
#define aecGrayBold		""      // White
#define aecWhiteBold	""      // White
#define aecMaroonBold	""      // Red
#define aecRedBold		""      // Red
#define aecGreenBold	""      // Green
#define aecLimeBold		""      // Green
#define aecOrangeBold	""      // Yellow
#define aecYellowBold	""      // Yellow
#define aecNavyBold		""      // Blue
#define aecBlueBold		""      // Blue
#define aecVioletBold	""      // Purple
#define aecPurpleBold	""      // Purple
#define aecTealBold		""      // Cyan
#define aecCyanBold		""      // Cyan

	// Background
#define aecOnBlack		""       // Black
#define aecOnCoal		""		 // Black
#define aecOnGray		""       // White
#define aecOnWhite		""		 // White
#define aecOnMaroon		""       // Red
#define aecOnRed		""       // Red
#define aecOnGreen		""       // Green
#define aecOnLime		""		 // Green
#define aecOnOrange		""       // Yellow
#define aecOnYellow		""		 // Yellow
#define aecOnNavy		""       // Blue
#define aecOnBlue		""		 // Blue
#define aecOnViolet		""       // Purple
#define aecOnPurple		""		 // Purple
#define aecOnTeal		""       // Cyan
#define aecOnCyan		""		 // Cyan

	// Underline
#define aecBlackUnder	""       // Black
#define aecGrayUnder	""       // White
#define aecMaroonUnder	""       // Red
#define aecGreenUnder	""       // Green
#define aecOrangeUnder	""       // Yellow
#define aecNavyUnder	""       // Blue
#define aecVioletUnder	""       // Purple
#define aecTealUnder	""       // Cyan

#else

#define aecReset "\x1b[0m"       // Text Reset

// Regular Colors
#define aecBlack "\x1b[30m"        // Black
#define aecCoal "\x1b[90m"       // Black
#define aecGray "\x1b[37m"        // White
#define aecWhite "\x1b[97m"       // White
#define aecMaroon "\x1b[31m"          // Red
#define aecRed "\x1b[91m"         // Red
#define aecGreen "\x1b[32m"        // Green
#define aecLime "\x1b[92m"       // Green
#define aecOrange "\x1b[33m"       // Yellow
#define aecYellow "\x1b[93m"      // Yellow
#define aecNavy "\x1b[34m"         // Blue
#define aecBlue "\x1b[94m"        // Blue
#define aecViolet "\x1b[35m"       // Purple
#define aecPurple "\x1b[95m"      // Purple
#define aecTeal "\x1b[36m"         // Cyan
#define aecCyan "\x1b[96m"        // Cyan

#define aecBlackBold "\x1b[1;30m"       // Black
#define aecCoalBold "\x1b[1;90m"      // Black
#define aecGrayBold "\x1b[1;37m"       // White
#define aecWhiteBold "\x1b[1;97m"      // White
#define aecMaroonBold "\x1b[1;31m"         // Red
#define aecRedBold "\x1b[1;91m"        // Red
#define aecGreenBold "\x1b[1;32m"       // Green
#define aecLimeBold "\x1b[1;92m"      // Green
#define aecOrangeBold "\x1b[1;33m"      // Yellow
#define aecYellowBold "\x1b[1;93m"     // Yellow
#define aecNavyBold "\x1b[1;34m"        // Blue
#define aecBlueBold "\x1b[1;94m"       // Blue
#define aecVioletBold "\x1b[1;35m"      // Purple
#define aecPurpleBold "\x1b[1;95m"     // Purple
#define aecTealBold "\x1b[1;36m"        // Cyan
#define aecCyanBold "\x1b[1;96m"       // Cyan

// Background
#define aecOnBlack "\x1b[40m"       // Black
#define aecOnCoal "\x1b[100m"   // Black
#define aecOnGray "\x1b[47m"       // White
#define aecOnWhite "\x1b[107m"   // White
#define aecOnMaroon "\x1b[41m"         // Red
#define aecOnRed "\x1b[101m"     // Red
#define aecOnGreen "\x1b[42m"       // Green
#define aecOnLime "\x1b[102m"   // Green
#define aecOnOrange "\x1b[43m"      // Yellow
#define aecOnYellow "\x1b[103m"  // Yellow
#define aecOnNavy "\x1b[44m"        // Blue
#define aecOnBlue "\x1b[104m"    // Blue
#define aecOnViolet "\x1b[45m"      // Purple
#define aecOnPurple "\x1b[105m"  // Purple
#define aecOnTeal "\x1b[46m"        // Cyan
#define aecOnCyan "\x1b[106m"    // Cyan

// Underline
#define aecBlackUnder "\x1b[4;30m"       // Black
#define aecGrayUnder "\x1b[4;37m"       // White
#define aecMaroonUnder "\x1b[4;31m"      // Red
#define aecGreenUnder "\x1b[4;32m"       // Green
#define aecOrangeUnder "\x1b[4;33m"      // Yellow
#define aecNavyUnder "\x1b[4;34m"        // Blue
#define aecVioletUnder "\x1b[4;35m"      // Purple
#define aecTealUnder "\x1b[4;36m"        // Cyan

#endif

}

}
