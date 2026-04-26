#!/bin/bash

# Script de instalação de dependências para o Gnome Themes Manager
# Suporta: Fedora, Ubuntu/Debian, Arch Linux

set -e

echo "🔍 Identificando a distribuição..."

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    echo "❌ Não foi possível identificar a distribuição."
    exit 1
fi

install_fedora() {
    echo "📦 Instalando dependências no Fedora..."
    sudo dnf groupinstall -y "Development Tools"
    sudo dnf install -y meson ninja-build glib2-devel ncurses-devel zlib libarchive-devel libarchive-devel
}

install_ubuntu() {
    echo "📦 Instalando dependências no Ubuntu/Debian..."
    sudo apt update
    sudo apt install -y build-essential meson ninja-build libglib2.0-dev libncurses5-dev zlib libarchive1g-dev libarchive-dev
}

install_arch() {
    echo "📦 Instalando dependências no Arch Linux..."
    sudo pacman -Syu --needed --noconfirm base-devel meson ninja glib2 ncurses zlib libarchive
}

case $OS in
    fedora)
        install_fedora
        ;;
    ubuntu|debian|pop|mint)
        install_ubuntu
        ;;
    arch)
        install_arch
        ;;
    *)
        echo "⚠️  Distribuição '$OS' não suportada automaticamente."
        echo "Por favor, instale manualmente: meson, ninja, glib2, ncurses, zlib libarchive"
        exit 1
        ;;
esac

echo "✅ Dependências instaladas com sucesso!"
echo "Para compilar o projeto, use:"
echo "  meson setup build"
echo "  meson compile -C build"
