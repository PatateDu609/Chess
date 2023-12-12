#!/bin/bash

mkdir -p dark light

for piece in k q r b p n; do
	case $piece in
		k)
		piece_name="king"
		;;	
		q)
		piece_name="queen"
		;;	
		r)
		piece_name="rook"
		;;	
		b)
		piece_name="bishop"
		;;	
		p)
		piece_name="pawn"
		;;	
		n)
		piece_name="knight"
		;;	
	esac

	for color in l d; do
		case $color in
			l)
			color_name="light"
			;;
			d)
			color_name="dark"
			;;
		esac

		file="$color_name/$piece_name.svg"

		curl https://commons.wikimedia.org/wiki/Category:SVG_chess_pieces\#/media/File:Chess_"$piece""$color"t45.svg > $file
	done
done
