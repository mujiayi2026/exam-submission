# 实验室实践考核代码提交仓库

## 考核题目

届时发布

---

## 提交方式

每位同学使用**自己的分支**提交代码，分支命名格式：`姓名拼音`（全小写，无空格），例如 `zhangsan`。

---

## 完整操作流程

> 以下步骤假设你已经安装了 Git。如果没有，请先下载安装：https://git-scm.com/downloads

### 第一步：Fork 仓库

1. 打开仓库页面：https://github.com/mujiayi2026/exam-submission
2. 点击右上角的 **Fork** 按钮，将仓库复制到你自己的 GitHub 账号下
3. Fork 完成后，你的仓库地址为：`https://github.com/<你的用户名>/exam-submission`

### 第二步：克隆你自己的仓库到本地

打开终端（Git Bash 或 CMD），执行：

```bash
git clone https://github.com/<你的用户名>/exam-submission.git
cd exam-submission
```

### 第三步：创建你的分支

```bash
git checkout -b zhangsan
```

将 `zhangsan` 替换为你姓名的拼音。

### 第四步：放入你的代码

将你的 Keil 工程文件夹复制到仓库根目录，目录结构**示例**：

```
exam-submission/
├── README.md
├── .gitignore
├── 考核题目.md
├── 题目一/      # 你的工程文件夹
│   ├── （你网上搜索到的文档）
│   ├── （你的工程文件夹）
│   ├── （你的思路）
│   ├── （以上均为示例）
└── 题目三/        # 如果做了多题
    ├── ...
    ├── ...
    └── ...
```

### 第五步：提交并推送

```bash
git add .
git commit -m "完成题目一、题目三"
git push origin zhangsan
```

### 第六步：创建 Pull Request

1. 打开你 Fork 的仓库页面：`https://github.com/<你的用户名>/exam-submission`
2. 点击 **Pull requests** → **New pull request**
3. base 仓库选择 `mujiayi2026/exam-submission` 的 `main` 分支，head 选择你自己的分支
4. 点击 **Create pull request**，标题填写你的姓名和完成的题目

---

## GitHub 访问问题解决方案

国内访问 GitHub 可能较慢或无法连接，请自行挂梯子或者搜索解决方案，网上开源方案非常多。

## 注意事项

- 每人只操作自己的分支，**不要**推送到 `main` 分支
- 代码需自己编写，禁止使用 AI 工具直接生成
- 确保 Keil 工程可独立编译通过（0 Error，0 Warning）
- 推送前请确认 `.gitignore` 已正确排除编译产物（.o、.hex 等）

---

## 常见问题

**Q: 推送时报错 `permission denied`？**
A: 确认你 Fork 了仓库，并且推送的是自己的仓库地址。如果用 HTTPS 方式，检查是否需要输入 GitHub 用户名和密码（或 Token）。

**Q: 推送时报错 `Failed to connect to github.com`？**
A: 网络问题，请参考上方「GitHub 访问问题解决方案」配置代理或使用镜像。

**Q: 不小心推错了分支？**
A: 联系助教处理，不要自行 force push。

**Q: 忘记分支名怎么查看？**
A: 运行 `git branch` 查看本地分支，`git branch -r` 查看远程分支。

**Q: `.gitignore` 没有生效，编译产物还是被提交了？**
A: 如果文件已经被 Git 追踪，`.gitignore` 不会自动生效。执行以下命令清除缓存：
```bash
git rm -r --cached .
git add .
git commit -m "修复 .gitignore"
```

**Q: 如何更新 Fork 仓库（同步助教的最新内容）？**
A: 在 GitHub 你的仓库页面点击 **Sync fork** → **Update branch` 即可。
