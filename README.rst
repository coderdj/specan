Spectral Analyzer for .pff Files
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

Change script parameters if wanted. Then run 

>make
>./specan --file={path} --threshold={threshold}

The file parameter is necessary and can either be an absolute path or
a stub. The threshold is optional and defaults to zero if non provided.

Credit
------

Written by Daniel Coderre, LHEP, Uni Bern. ROOT is an open source
package developed and distributed by CERN (see root.cern.ch). 