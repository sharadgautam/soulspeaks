module.exports = {
  devServer: {
    proxy: {
      '/api': {
        target: 'http://localhost:9080',
        changeOrigin: true,
      },
    },
  },
}; 