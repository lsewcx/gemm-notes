import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  base: '/gemm-notes/',
  title: "GEMM 学习笔记",
  description: "个人学习记录与知识库",
  markdown: {
    math: true  // 启用 LaTeX 公式渲染（markdown-it-mathjax3）
  },
  vite: {
    build: {
      target: 'esnext'
    },
    optimizeDeps: {
      esbuildOptions: {
        target: 'esnext',
        supported: {
          destructuring: true
        }
      }
    },
    esbuild: {
      supported: {
        destructuring: true
      }
    }
  },
  themeConfig: {
    // 导航栏
    nav: [
      { text: '首页', link: '/' },
      { text: '笔记', link: '/notes/' }
    ],

    // 侧边栏
    sidebar: [
      {
        text: '笔记目录',
        items: [
          { text: '起步', link: '/notes/' },
          { text: '01 · 最朴素的 GEMM', link: '/notes/01-naive-gemm' }
        ]
      }
    ],

    // 搜索
    search: {
      provider: 'local'
    }
  }
})
