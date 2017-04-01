# C Project: FASTA sequences manipulation

This project is about working on protein and nucleic sequences, stored in FASTA formatted files, in order to make specific searches (by gene name, sequence, position in the genome and sub-string), translations and later calculate distances or make sequencing simulations.

## Authors

Sophie LEMATRE & Gabriel CRETIN

## Install and execute

Compilation:

`make`

Clean *.o files:

`make clean`

Clean *.o and executable file:

`make wipeout`

Execute project:

`./fasta_project`

## Implemented functions

Search by:

- Gene name
- Sequence
- Position in the genome
- Sub sequence

Search in the dictionary-tree:

- Full sequences
- Number of sequences starting by a given prefix

Translation: 

Nucleic sequences are translated into their proteic sequences. Output is in `data/translation.txt`
