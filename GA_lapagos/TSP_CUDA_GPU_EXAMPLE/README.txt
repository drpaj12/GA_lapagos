

Setting up a Colab - Notebook

Followed steps from: https://www.wikihow.com/Run-CUDA-C-or-C%2B%2B-on-Jupyter-(Google-Colab)

Jupiter notebook - Change to GPU - Runtime 

------ Install Cuda

!apt-get --purge remove cuda nvidia* libnvidia-*
!dpkg -l | grep cuda- | awk '{print $2}' | xargs -n1 dpkg --purge
!apt-get remove cuda-*
!apt autoremove
!apt-get update

!wget https://developer.nvidia.com/compute/cuda/9.2/Prod/local_installers/cuda-repo-ubuntu1604-9-2-local_9.2.88-1_amd64 -O cuda-repo-ubuntu1604-9-2-local_9.2.88-1_amd64.deb
!dpkg -i cuda-repo-ubuntu1604-9-2-local_9.2.88-1_amd64.deb
!apt-key add /var/cuda-repo-9-2-local/7fa2af80.pub
!apt-get update
!apt-get install cuda-9.2

----- CHECK if CUDA compiler good.  Note - tutorial also gives a bit of code you can test
!nvcc --version

----- END Install Cuda

----- To create flat C file from GA_lapagos code base
Used profiler
1. Ran GA_lapagos with config
2. "gprof GA_lapagos >& profile_out.txt"
Used profile file to create flattened C functions.  In vim ":tag function_name" to get the code and copied function
Next ran "ctags -x --c-kinds=fp GA_galapagos_flat_gpu.c" to get a list of functions to add as C prototypes
But in the end, just copied hardware example and made own 1d data structure generator

Need to take each code base and replace NUM_THREADS with actual number before copying into Colab.



