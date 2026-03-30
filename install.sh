#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPOSE_CMD=(docker compose)

log() {
  printf '[install] %s\n' "$1"
}

warn() {
  printf '[install][warn] %s\n' "$1" >&2
}

run_as_root() {
  if [[ "${EUID}" -eq 0 ]]; then
    "$@"
  else
    if ! command -v sudo >/dev/null 2>&1; then
      warn "sudo is required for installing system packages."
      exit 1
    fi
    sudo "$@"
  fi
}

append_if_missing() {
  local line="$1"
  local file="$2"
  grep -Fxq "$line" "$file" || printf '%s\n' "$line" >> "$file"
}

install_os_packages() {
  if [[ ! -f /etc/debian_version ]]; then
    warn "Automatic dependency installation supports only Ubuntu/Debian."
    warn "Install dependencies manually and re-run this script."
    return
  fi

  log "Installing OS dependencies (cmake, curl, docker, psql client, etc.)..."
  run_as_root apt update
  run_as_root apt install -y \
    build-essential \
    cmake \
    pkg-config \
    ninja-build \
    git \
    curl \
    ca-certificates \
    libcurl4-openssl-dev \
    libssl-dev \
    zlib1g-dev \
    libboost-system-dev \
    libboost-program-options-dev \
    docker.io \
    docker-compose-plugin \
    openssl \
    postgresql-client
}

has_tgbot_cmake_config() {
  local cmake_paths=(
    "/usr/local/lib/cmake/TgBot/TgBotConfig.cmake"
    "/usr/lib/cmake/TgBot/TgBotConfig.cmake"
    "/usr/lib/x86_64-linux-gnu/cmake/TgBot/TgBotConfig.cmake"
  )

  for path in "${cmake_paths[@]}"; do
    if [[ -f "$path" ]]; then
      return 0
    fi
  done
  return 1
}

install_tgbot_cpp_if_needed() {
  if has_tgbot_cmake_config; then
    log "tgbot-cpp is already installed."
    return
  fi

  log "Installing tgbot-cpp from source..."
  local tmpdir
  tmpdir="$(mktemp -d)"
  trap 'rm -rf "$tmpdir"' EXIT

  git clone --depth 1 https://github.com/reo7sp/tgbot-cpp.git "$tmpdir/tgbot-cpp"
  cmake -S "$tmpdir/tgbot-cpp" -B "$tmpdir/tgbot-cpp/build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_SHARED_LIBS=ON
  cmake --build "$tmpdir/tgbot-cpp/build" --parallel
  run_as_root cmake --install "$tmpdir/tgbot-cpp/build"

  rm -rf "$tmpdir"
  trap - EXIT
}

prepare_project_files() {
  log "Preparing local project files..."
  mkdir -p "$PROJECT_ROOT/volume/postgres" "$PROJECT_ROOT/secrets"
  chmod 700 "$PROJECT_ROOT/secrets"

  if [[ ! -s "$PROJECT_ROOT/secrets/postgres_password.txt" ]]; then
    openssl rand -base64 32 > "$PROJECT_ROOT/secrets/postgres_password.txt"
    log "Generated secrets/postgres_password.txt"
  fi
  chmod 600 "$PROJECT_ROOT/secrets/postgres_password.txt"

  if [[ ! -f "$PROJECT_ROOT/.env.postgres" ]]; then
    cat > "$PROJECT_ROOT/.env.postgres" <<'EOF'
POSTGRES_USER=tg_user
POSTGRES_DB=tg_db
EOF
    log "Created .env.postgres"
  fi

  if [[ ! -f "$PROJECT_ROOT/include/config.h" ]]; then
    cat > "$PROJECT_ROOT/include/config.h" <<'EOF'
#pragma once

#define BOT_TOKEN "PASTE_TELEGRAM_BOT_TOKEN_HERE"
EOF
    warn "Created include/config.h. Set BOT_TOKEN before running the bot."
  fi

  touch "$PROJECT_ROOT/.gitignore" "$PROJECT_ROOT/.dockerignore"
  append_if_missing ".env" "$PROJECT_ROOT/.gitignore"
  append_if_missing ".env.*" "$PROJECT_ROOT/.gitignore"
  append_if_missing "!.env.example" "$PROJECT_ROOT/.gitignore"
  append_if_missing "secrets/" "$PROJECT_ROOT/.gitignore"
  append_if_missing "volume/postgres/" "$PROJECT_ROOT/.gitignore"

  append_if_missing "secrets/" "$PROJECT_ROOT/.dockerignore"
  append_if_missing ".env" "$PROJECT_ROOT/.dockerignore"
  append_if_missing ".env.*" "$PROJECT_ROOT/.dockerignore"
  append_if_missing "!.env.example" "$PROJECT_ROOT/.dockerignore"
}

resolve_compose_cmd() {
  if docker compose version >/dev/null 2>&1; then
    COMPOSE_CMD=(docker compose)
    return 0
  fi
  if command -v docker-compose >/dev/null 2>&1; then
    COMPOSE_CMD=(docker-compose)
    return 0
  fi
  return 1
}

start_postgres_container() {
  if ! command -v docker >/dev/null 2>&1; then
    warn "Docker CLI is not available, skipping PostgreSQL startup."
    return
  fi
  if ! resolve_compose_cmd; then
    warn "docker compose is not available, skipping PostgreSQL startup."
    return
  fi
  if ! docker info >/dev/null 2>&1; then
    warn "Docker daemon is not running, skipping PostgreSQL startup."
    return
  fi

  log "Starting PostgreSQL container..."
  (
    cd "$PROJECT_ROOT"
    "${COMPOSE_CMD[@]}" up -d postgres
  )
}

build_project() {
  log "Configuring project..."
  cmake -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/build"
  log "Building project..."
  cmake --build "$PROJECT_ROOT/build" --parallel
}

main() {
  install_os_packages
  install_tgbot_cpp_if_needed
  prepare_project_files
  start_postgres_container
  build_project

  log "Done."
  log "Next: put bot token into include/config.h, then run ./build/MyTelegramBot"
}

main "$@"
