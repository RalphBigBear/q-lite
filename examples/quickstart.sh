#!/bin/bash
# Q-Lite Quick Start Demo
# Inspired by nanochat's speedrun.sh - end-to-end testing in one script
#
# This script will:
# 1. Check if Ollama is running, start if not
# 2. Pull a test model (qwen2.5:0.5b)
# 3. Build Q-Lite (if not exists)
# 4. Start Q-Lite gateway
# 5. Run a test request

set -e  # Exit on error

echo "╔══════════════════════════════════════════╗"
echo "║     Q-Lite Quick Start Demo                ║"
echo "╚══════════════════════════════════════════╝"
echo ""

# Color codes
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 1. Check if Ollama is running
echo -e "${BLUE}[1/5] Checking Ollama...${NC}"
if pgrep -x "ollama" > /dev/null; then
    echo -e "${GREEN}✓ Ollama is already running${NC}"
else
    echo -e "${YELLOW}⚠ Ollama not running, starting...${NC}"
    ollama serve > /dev/null 2>&1 &
    OLLAMA_PID=$!
    echo -e "${GREEN}✓ Ollama started (PID: $OLLAMA_PID)${NC}"
    sleep 5  # Wait for Ollama to be ready
fi
echo ""

# 2. Pull test model
echo -e "${BLUE}[2/5] Pulling test model (qwen2.5:0.5b)...${NC}"
if ollama list | grep -q "qwen2.5:0.5b"; then
    echo -e "${GREEN}✓ Model already exists${NC}"
else
    echo "Downloading... (this may take a minute)"
    ollama pull qwen2.5:0.5b
    echo -e "${GREEN}✓ Model downloaded${NC}"
fi
echo ""

# 3. Build Q-Lite
echo -e "${BLUE}[3/5] Building Q-Lite...${NC}"
if [ -f "./q-lite" ]; then
    echo -e "${GREEN}✓ Q-Lite binary exists${NC}"
else
    echo "Compiling..."
    make
    echo -e "${GREEN}✓ Q-Lite built successfully${NC}"
fi
echo ""

# 4. Start Q-Lite
echo -e "${BLUE}[4/5] Starting Q-Lite gateway on :8080...${NC}"
./q-lite --target desktop --port 8080 &
Q_LITE_PID=$!
sleep 3  # Wait for Q-Lite to be ready
echo -e "${GREEN}✓ Q-Lite started (PID: $Q_LITE_PID)${NC}"
echo ""

# 5. Run test request
echo -e "${BLUE}[5/5] Running test request...${NC}"
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Request: POST /api/generate"
echo "Model:   qwen2.5:0.5b"
echo "Prompt:  'Hello, Q-Lite!'"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

RESPONSE=$(curl -s -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{
    "model": "qwen2.5:0.5b",
    "prompt": "Hello, Q-Lite!",
    "stream": false
  }')

echo "Response:"
echo "$RESPONSE" | python3 -m json.tool 2>/dev/null || echo "$RESPONSE"
echo ""

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo -e "${GREEN}╔══════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║  ✅ Demo Complete! Q-Lite is running.    ║${NC}"
echo -e "${GREEN}╠══════════════════════════════════════════╣${NC}"
echo -e "${GREEN}║  PID: $Q_LITE_PID                          ║${NC}"
echo -e "${GREEN}║  URL: http://localhost:8080              ║${NC}"
echo -e "${GREEN}║                                              ║${NC}"
echo -e "${GREEN}║  Test more:                                  ║${NC}"
echo -e "${GREEN}║    curl http://localhost:8080/health       ║${NC}"
echo -e "${GREEN}║                                              ║${NC}"
echo -e "${GREEN}║  Stop: kill $Q_LITE_PID                     ║${NC}"
echo -e "${GREEN}╚══════════════════════════════════════════╝${NC}"
echo ""
echo "💡 Tip: Press Ctrl+C to stop Q-Lite"
echo ""

# Wait for user interrupt
wait $Q_LITE_PID
