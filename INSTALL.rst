Installing SYMBIOSES 
=====================

# Setting up the build environment on stallo::
    $ cd /path/to/symbioses
    $ . tools/stallo-buildenv.sh

    Or alternatively by hand:

    $ module load git 
    $ module load cmake
    $ module load python/2.7.2
    $ module load openmpi
    $ module load netcdf/4.1.3
    $ module load boost
    $ module load eigen
# Cloning the code::
    $ git clone git@hub.symbioses.no:symbioses
    $ cd symbioses
# Configuring and building the code::
    $ ./setup --host=stallo
    $ ./tools/stallo-setup-testdata.sh # Alternatively, see below
    $ cd build/
    $ make
# Testing the code::
    $ make test
# Testing and submitting to the Dashboard::
    $ make Experimental
# Setting up the test data::
    $ cd data/
    $ ln -s /projects/symbioses/symbioses_input input

