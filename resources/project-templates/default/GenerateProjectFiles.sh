#!/bin/sh
# Generates the project files using the engine's Premake: Xcode on macOS, makefiles elsewhere

if [ -z "$HANGOVER_BIN" ]; then
	echo "HANGOVER_BIN is not set. Launch liquor or the Hangover editor once to register the engine."
	exit 1
fi

case "$(uname)" in
	Darwin) action=xcode4 ;;
	*)      action=gmake ;;
esac

"$HANGOVER_BIN/premake/premake5" --file="$(dirname "$0")/premake5.lua" "$action"
