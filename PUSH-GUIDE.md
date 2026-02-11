# Q-Lite GitHub 推送指南

## ✅ 本地仓库已创建

**位置**: `/Users/ralph/.openclaw/workspace/projects/q-lite`

**已提交文件**:
- README.md (专业的介绍 + Karpathy 致谢)
- LICENSE (MIT)
- Makefile
- src/main.c (基础程序)
- src/config.h (配置头文件)
- src/http.c (占位符)
- src/ollama.c (占位符)
- .gitignore

---

## 🚀 推送到 GitHub

### 方法 1: 使用 GitHub CLI (推荐)

```bash
cd /Users/ralph/.openclaw/workspace/projects/q-lite
gh repo set-default RalphBigBear/q-lite
git remote set-url origin https://github.com/RalphBigBear/q-lite.git
git push -u origin main
```

### 方法 2: 手动推送（如果 CLI 失败）

```bash
cd /Users/ralph/.openclaw/workspace/projects/q-lite

# 设置 credential helper (Mac)
git config --global credential.helper osxkeychain

# 推送 (会弹出浏览器登录)
git push -u origin main
```

### 方法 3: 使用 SSH (如果 HTTPS 失败)

```bash
# 1. 生成 SSH key (如果还没有)
ssh-keygen -t ed25519 -C "your_email@example.com"

# 2. 添加到 GitHub (Settings → SSH and GPG keys)
cat ~/.ssh/id_ed25519.pub | pbcopy

# 3. 切换到 SSH URL
git remote set-url origin git@github.com:RalphBigBear/q-lite.git

# 4. 推送
git push -u origin main
```

---

## 📋 推送后的检查清单

访问 https://github.com/RalphBigBear/q-lite 确认：

- [ ] README.md 显示完整
- [ ] "Inspired By" 部分包含 Karpathy
- [ ] LICENSE 显示为 MIT
- [ ] 文件结构正确 (src/, Makefile)
- [ ] 无敏感信息 (凭证、密钥等)

---

## 🎯 下一步 (推送成功后)

### Step 1: 添加 Topics

```bash
gh repo edit RalphBigBear/q-lite \
  --add-topic llm,edge-computing,embedded,gateway,minimal,c,lightweight,inference
```

### Step 2: 创建 GitHub Issues

**Issue #1: Implement HTTP Server**
```markdown
## Task: Implement HTTP Server (src/http.c)

### Requirements
- Single-threaded event loop
- Support GET/POST methods
- JSON request parsing
- <100KB memory footprint

### References
- libmicrohttpd documentation
- PicoClaw HTTP implementation

### Estimated Time: 1-2 hours
```

**Issue #2: Implement Ollama Client (src/ollama.c)**
```markdown
## Task: Implement Ollama API Client

### Requirements
- HTTP client for Ollama /api/generate
- Request/response JSON parsing
- Error handling

### API Reference
- https://github.com/ollama/ollama/blob/main/docs/api.md

### Estimated Time: 1-2 hours
```

---

## ✅ 可信度检查

推送前最后检查：

- [x] README 有 Karpathy 致谢
- [x] README 有 "Inspired By" 部分
- [x] 有具体数字（<1MB, <100KB）
- [x] 代码风格专业
- [x] LICENSE 是 MIT
- [x] 无高维信息泄露
- [x] 项目描述清晰

---

**准备推送！主人，选择一个方法尝试。**

如果遇到认证问题，GitHub 会弹出浏览器让你登录授权。
