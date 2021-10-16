// vue.config.js

/**
 * @type {import('@vue/cli-service').ProjectOptions}
 */
module.exports = {
    productionSourceMap: false,

    chainWebpack: config => {
		config.optimization.delete('splitChunks');
	},

    css: {
		extract: {
			filename: '[name].css',
			chunkFilename: '[name].css',
		},
	},

    configureWebpack: {
		output: {
			filename: '[name].js',
			chunkFilename: '[name].js',
		}
	},

    transpileDependencies: [
      'vuetify'
    ]
};
