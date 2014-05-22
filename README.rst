Spectral Analyzer for .pbf Files
################################

Intro
-----

Simple spectral analyzer. Integrates the highest peak in each
event and plots.

Requirements
------------

You need ROOT (root.cern.ch). You should be able to type "root" into a
console and get dropped into the shell. You also need libpbf
(https://github.com/coderdj/libpbf)

Usage
-----

Run::

  make
  ./specan --file={path} 

The file parameter is necessary and can either be an absolute path or
a stub. There are also another few command-line options.::

  --threshold={int}
  --module={int}
  --channel={int}
  -b 

The threshold is optional and defaults to zero if not provided. Module and channel allow making the spectrum for a single module and channel. The -b flag tells the program to continuously populate and draw the spectrum online (otherwise it is just dumped to file). 

Credit
------

Written by Daniel Coderre, LHEP, Uni Bern. ROOT is an open source
package developed and distributed by CERN (see root.cern.ch). 
