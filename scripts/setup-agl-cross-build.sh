#!/bin/bash
# AGL Cross-Compilation Build Setup Script
# Team04 - DrivaPi
# 
# This script automates the setup of the AGL build environment
# with the meta-cross layer for Rust cross-compilation.

set -e  # Exit on error

# Configuration
AGL_BRANCH="trout"
AGL_MACHINE="raspberrypi5"
AGL_FEATURES="agl-all-features agl-devel"
WORKSPACE_DIR="${HOME}/agl-workspace"
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
META_CROSS_PATH="${REPO_ROOT}/meta-cross"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Helper functions
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check prerequisites
check_prerequisites() {
    log_info "Checking prerequisites..."
    
    # Check required commands
    for cmd in git python3 wget repo; do
        if ! command -v $cmd &> /dev/null; then
            log_error "$cmd is not installed. Please install it first."
            exit 1
        fi
    done
    
    # Check disk space (100GB minimum)
    AVAILABLE_SPACE=$(df -BG "${HOME}" | tail -1 | awk '{print $4}' | sed 's/G//')
    if [ "$AVAILABLE_SPACE" -lt 100 ]; then
        log_warn "Less than 100GB free space available. Build may fail."
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
    
    log_info "Prerequisites check passed."
}

# Initialize AGL workspace
init_agl_workspace() {
    log_info "Initializing AGL workspace..."
    
    if [ -d "$WORKSPACE_DIR" ]; then
        log_warn "Workspace directory already exists: $WORKSPACE_DIR"
        read -p "Remove and re-initialize? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rm -rf "$WORKSPACE_DIR"
        else
            log_info "Using existing workspace."
            return 0
        fi
    fi
    
    mkdir -p "$WORKSPACE_DIR"
    cd "$WORKSPACE_DIR"
    
    log_info "Running repo init for AGL $AGL_BRANCH..."
    repo init -b "$AGL_BRANCH" -u https://gerrit.automotivelinux.org/gerrit/AGL/AGL-repo
    
    log_info "Syncing AGL repositories (this may take a while)..."
    repo sync -j4
    
    log_info "AGL workspace initialized successfully."
}

# Setup build environment
setup_build_env() {
    log_info "Setting up build environment for $AGL_MACHINE..."
    
    cd "$WORKSPACE_DIR"
    
    # Source AGL setup script
    if [ ! -f "meta-agl/scripts/aglsetup.sh" ]; then
        log_error "aglsetup.sh not found. AGL workspace may be incomplete."
        exit 1
    fi
    
    log_info "Running aglsetup.sh..."
    source meta-agl/scripts/aglsetup.sh -m "$AGL_MACHINE" $AGL_FEATURES
    
    log_info "Build environment setup complete."
}

# Add meta-cross layer
add_meta_cross_layer() {
    log_info "Adding meta-cross layer..."
    
    if [ ! -d "$META_CROSS_PATH" ]; then
        log_error "meta-cross layer not found at: $META_CROSS_PATH"
        exit 1
    fi
    
    cd "$WORKSPACE_DIR/build"
    
    # Check if layer already added
    if grep -q "meta-cross" conf/bblayers.conf 2>/dev/null; then
        log_warn "meta-cross layer already added to bblayers.conf"
    else
        log_info "Adding meta-cross to bblayers.conf..."
        bitbake-layers add-layer "$META_CROSS_PATH"
    fi
    
    # Verify layer was added
    if bitbake-layers show-layers | grep -q "cross"; then
        log_info "meta-cross layer successfully added."
    else
        log_error "Failed to add meta-cross layer."
        exit 1
    fi
}

# Configure build settings
configure_build() {
    log_info "Configuring build settings..."
    
    cd "$WORKSPACE_DIR/build"
    
    # Detect number of CPU cores
    NUM_CORES=$(nproc)
    PARALLEL_MAKE=$((NUM_CORES > 1 ? NUM_CORES - 1 : 1))
    
    # Backup original local.conf
    if [ ! -f "conf/local.conf.orig" ]; then
        cp conf/local.conf conf/local.conf.orig
    fi
    
    # Append optimized settings
    cat >> conf/local.conf <<EOF

# --- Team04 DrivaPi Custom Settings ---
# Performance optimization
PARALLEL_MAKE = "-j ${PARALLEL_MAKE}"
BB_NUMBER_THREADS = "${PARALLEL_MAKE}"

# Disk space monitoring
BB_DISKMON_DIRS = "\\
    STOPTASKS,\${TMPDIR},1G,100K \\
    STOPTASKS,\${DL_DIR},1G,100K \\
    STOPTASKS,\${SSTATE_DIR},1G,100K"

# Enable buildhistory for debugging
INHERIT += "buildhistory"
BUILDHISTORY_COMMIT = "1"

# Rust-specific settings (handled by meta-cross)
# CARGO_BUILD_FLAGS are set in recipe
EOF
    
    log_info "Build configuration complete."
}

# Display summary
display_summary() {
    echo ""
    echo "========================================"
    log_info "AGL Cross-Compilation Setup Complete!"
    echo "========================================"
    echo ""
    echo "Workspace:     $WORKSPACE_DIR"
    echo "Build Dir:     $WORKSPACE_DIR/build"
    echo "Meta-cross:    $META_CROSS_PATH"
    echo "Machine:       $AGL_MACHINE"
    echo "AGL Branch:    $AGL_BRANCH"
    echo ""
    echo "Next steps:"
    echo "  1. cd $WORKSPACE_DIR/build"
    echo "  2. source ../meta-agl/scripts/aglsetup.sh -m $AGL_MACHINE $AGL_FEATURES"
    echo "  3. bitbake controller          # Build Rust controller app"
    echo "  4. bitbake agl-demo-platform   # Build full AGL image"
    echo ""
    echo "For more information, see:"
    echo "  - docs/software/agl/yocto-cross-compilation.md"
    echo "  - meta-cross/README.md"
    echo ""
}

# Main execution
main() {
    echo "========================================"
    echo "AGL Cross-Compilation Setup"
    echo "Team04 - DrivaPi"
    echo "========================================"
    echo ""
    
    check_prerequisites
    init_agl_workspace
    setup_build_env
    add_meta_cross_layer
    configure_build
    display_summary
}

# Run main function
main
