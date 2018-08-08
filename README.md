# Prot-SpaM

### Note
Currently this program only supports Linux.

### Compilation
cd into the root directory (containing the 'Makefile') and type:

```	make ```

### Run

```	./protspam [options] -l <filelist> ```

### Filelist

The program takes a plain text file containing the relative paths to each input
dataset. To create your 'filelist' simply type:

``` ls -1 path/to/input/* > filelist ```

This will list each file in specified directory, one file per line.

### Options
```
	-h/-?: print this help and exit
	-w <integer>: pattern weight (default 6)
	-d <integer>: number of don't-care positions (default 40)
	-s <integer>: the minimum score of a spaced-word match to be considered homologous (default: 0)
	-p <integer>: number of patterns used (default 5)
	-t <integer>: number of threads (default: omp_get_max_threads() )
	-o <integer>: filename for distance matrix (default: DMat)
	-l <filename>: specify a list of files to read as input (one inputfile per organism containing each sequence, seperated by headers)
```

### Sequence format:

Sequence must be in FASTA format. All protein sequences of one proteome must be contained in one FASTA file.

Example:
```
>Protein1
RAKSDLKEASDKE..
>Protein2
ATSDLAGTASDKE..
>Protein3
ARNCQEFGSDSDW..
..
```

### Citation:
```
Scientific publications using filtered spaced word matches should cite:

Leimeister, C. A., Schellhorn, J., Schoebel, S., Gerth, M., Bleidorn, C., & Morgenstern, B. (2018).
Prot-SpaM: Fast alignment-free phylogeny reconstruction based on whole-proteome sequences.
bioRxiv, 306142.
```

### Paper Abstract:
```
Word-based or "alignment-free" sequence comparison has become an active area of research in bioinformatics.
Recently, fast word-based algorithms have been proposed that are able to accurately estimate phylogenetic
distances between genomic DNA sequences without the need to calculate full sequence alignments. One of these
approaches is Filtered Spaced Word Matches. Herein, we extend this approach to estimate evolutionary distances
between species based on their complete or incomplete proteomes; our implementation is called Prot-SpaM.
We show that Prot-SpaM can accurately estimate phylogenetic distances, and that our program can be used to
calculate phylogenetic trees from whole proteomes in a matter of seconds.
For various groups of taxa, we show that trees calculated with Prot-SpaM are of high quality.
The source code of our software is available through Github: https://github.com/jschellh/ProtSpaM
```


### Contact:
jendrik.schellhorn@stud.uni-goettingen.de
