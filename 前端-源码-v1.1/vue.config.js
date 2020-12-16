// vue.config.js
module.exports = {
  chainWebpack: config => {
    config
      .plugin('html')
      .tap(args => {
        args[0].title= '无线门锁系统 - T162018'
        return args
      })
  }
}