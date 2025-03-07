# Use the official Ubuntu 24.04 as the base image
FROM ubuntu:24.04

# ============================
# Define Build-Time Arguments
# ============================

ARG MINIFORGE_NAME=Miniforge3
ARG MINIFORGE_VERSION=25.1.1-0
ARG TMP=/tmp
ARG PYFES_VERSION=2025.2.0

# ============================
# Set Home Directory
# ============================

ENV HOME=/home/appuser

# ============================
# Set Environment Variables
# ============================

ENV CONDA_DIR=${HOME}/miniforge3
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8
ENV PATH=${CONDA_DIR}/bin:${PATH}

# ============================
# Install System Dependencies
# ============================

# Set a restrictive umask for better security
RUN umask 0002 && \
    apt-get update && apt-get install -y \
        ca-certificates \
        curl \
        git \
    && rm -rf /var/lib/{apt,dpkg,cache,log}

# ============================
# Create a Non-Root User
# ============================

# Create a system group and user named 'appuser'
RUN groupadd -r appuser && \
    useradd -m -d /home/appuser -r -g appuser appuser

# ============================
# Set Directory Permissions
# ============================

# Ensure that the non-root user has the necessary permissions
RUN mkdir -p /app && \
    chown -R appuser:appuser /app && \
    mkdir -p /tests/python/dataset && \
    chown -R appuser:appuser /tests/python/dataset

# ============================
# Switch to Non-Root User
# ============================

USER appuser

# ============================
# Install Miniforge
# ============================

RUN curl -L https://github.com/conda-forge/miniforge/releases/download/${MINIFORGE_VERSION}/${MINIFORGE_NAME}-${MINIFORGE_VERSION}-Linux-$(uname -m).sh -o ${TMP}/miniforge.sh && \
    bash ${TMP}/miniforge.sh -b -p ${CONDA_DIR} && \
    rm ${TMP}/miniforge.sh && \
    # Initialize conda for the current user
    echo ". ${CONDA_DIR}/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "conda activate base" >> ~/.bashrc

# ============================
# Install Conda Packages
# ============================

# Install the required conda packages
RUN mamba install blas=*=mkl \
        binutils \
        boost-cpp \
        cartopy \
        cmake \
        cxx-compiler \
        eigen \
        gtest \
        ipython \
        make \
        matplotlib \
        netcdf4 \
        pytest \
        pyyaml \
        setuptools_scm \
        sphinx \
        sphinx-book-theme \
        sphinx-gallery \
        -y -c conda-forge

# ============================
# Install PyFES
# ============================

RUN curl -L https://github.com/CNES/aviso-fes/releases/download/${PYFES_VERSION}/pyfes-${PYFES_VERSION}.tar.gz -o ${TMP}/pyfes-${PYFES_VERSION}.tar.gz && \
    tar -xzf ${TMP}/pyfes-${PYFES_VERSION}.tar.gz -C ${TMP} && \
    cd ${TMP}/pyfes-${PYFES_VERSION} && \
    pip install . --no-deps -Cmkl=1 && \
    sphinx-build -b html docs/source /app/pyfes-docs && \
    cp -R examples /app/pyfes-examples && \
    mv /app/pyfes-examples/*.yml /app && \
    rm /app/pyfes-examples/*.txt && \
    cp -R tests/python/dataset/* /tests/python/dataset && \
    rm -rf ${TMP}/pyfes-${PYFES_VERSION}.tar.gz ${TMP}/pyfes-${PYFES_VERSION}

# ============================
# Clean Up Conda Cache
# ============================

RUN conda clean --tarballs --index-cache --packages --yes && \
find ${CONDA_DIR} -follow -type f -name '*.pyc' -delete && \
conda clean --force-pkgs-dirs --all --yes

# ============================
# Clean up pip cache
# ============================
RUN rm -rf /home/appuser/.cache/pip

# ============================
# Clean Up Temporary Files
# ============================

RUN rm -rf ${TMP}/*

# ============================
# Create a banner
# ============================

RUN echo "echo \"#########################################\"" >> ~/.bashrc && \
    echo "echo \"#                                       #\"" >> ~/.bashrc && \
    echo "echo \"#    Welcome to the PyFES ${PYFES_VERSION}      #\"" >> ~/.bashrc && \
    echo "echo \"#                                       #\"" >> ~/.bashrc && \
    echo "echo \"# Documentation available at:           #\"" >> ~/.bashrc && \
    echo "echo \"# https://cnes.github.io/aviso-fes/     #\"" >> ~/.bashrc && \
    echo "echo \"#                                       #\"" >> ~/.bashrc && \
    echo "echo \"#########################################\"" >> ~/.bashrc

# ============================
# Set Working Directory
# ============================

WORKDIR /app

# ============================
# Default Command
# ============================

CMD ["bash"]
