module.exports = {
  devServer: {
    proxy: {
      '/api': {
        target: 'http://172.22.220.74:9080',
        changeOrigin: true,
      },
    },
  },
}; 