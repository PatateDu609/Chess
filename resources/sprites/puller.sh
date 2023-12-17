#!/bin/bash

mkdir -p dark light

curl https://upload.wikimedia.org/wikipedia/commons/4/42/Chess_klt45.svg > light/king.svg
curl https://upload.wikimedia.org/wikipedia/commons/1/15/Chess_qlt45.svg> light/queen.svg
curl https://upload.wikimedia.org/wikipedia/commons/7/70/Chess_nlt45.svg > light/knight.svg
curl https://upload.wikimedia.org/wikipedia/commons/b/b1/Chess_blt45.svg > light/bishop.svg
curl https://upload.wikimedia.org/wikipedia/commons/7/72/Chess_rlt45.svg > light/rook.svg
curl https://upload.wikimedia.org/wikipedia/commons/4/45/Chess_plt45.svg > light/pawn.svg

curl https://upload.wikimedia.org/wikipedia/commons/f/f0/Chess_kdt45.svg > dark/king.svg
curl https://upload.wikimedia.org/wikipedia/commons/4/47/Chess_qdt45.svg > dark/queen.svg
curl https://upload.wikimedia.org/wikipedia/commons/e/ef/Chess_ndt45.svg > dark/knight.svg
curl https://upload.wikimedia.org/wikipedia/commons/9/98/Chess_bdt45.svg > dark/bishop.svg
curl https://upload.wikimedia.org/wikipedia/commons/f/ff/Chess_rdt45.svg > dark/rook.svg
curl https://upload.wikimedia.org/wikipedia/commons/c/c7/Chess_pdt45.svg > dark/pawn.svg
