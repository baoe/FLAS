### Overview
FLAS is software that makes self-correction for PacBio long reads with fast speed and high throughput.

### Copy right
FLAS is under the [Artistic License 2.0](http://opensource.org/licenses/Artistic-2.0).

### Short manual
1. System requirements

   FLAS is suitable for 32-bit or 64-bit machines with Linux operating systems. At least 4GB of system memory is recommended for correcting larger data sets.

2. Installation

   The [MECAT](https://github.com/xiaochuanle/MECAT) software and [Python(3.0+)](https://www.python.org/downloads/) are required to run FLAS.
   * The source files in 'src' folder can be complied to generate a 'bin' folder by running Makefile: `make`.
   * Put the 'bin' folders of MECAT and FLAS to your $PATH: `export PATH=PATH2MECATbin:$PATH` and `export PATH=PATH2FLASbin:$PATH`, respectively.

3. Input
   * Long reads in FASTA format.
   
4. Using FLAS

   ```
   FLAS long_reads.fasta [-options|-options]
   ```

   <p>Options (default value):<br>
   -c (100)<br>
   Coverage of the long reads.<br>
   -α1 (0.5)<br>
   Fraction value to distinguish between the cases (i)/(ii) and (iii).<br>
   -α2 (0.5)<br>
   Fraction value to distinguish between the cases (i) and (ii).<br>
   -β1 (0.05)<br>
   Difference value of alignment identities to find the correct aligned path.<br>
   -β2 (0.2)<br>
   Difference value of expected amounts of aligned long reads to find the correct aligned path.<br>

      
5. Outputs
   * Error corrected trimmed long reads.
   * Error corrected split long reads.


