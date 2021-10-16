<template>
	<v-icon
		:class="
			quality < 25 ? 'error--text' : (
				quality < 50 ? 'warning--text' : (
					quality < 50 ? 'info--text' : 'success--text'
				)
			)
		"
	>{{
		quality < 25 ? $icons.mdiWifiStrength1 : (
			quality < 50 ? $icons.mdiWifiStrength2 : (
				quality < 50 ? $icons.mdiWifiStrength3 : $icons.mdiWifiStrength4
			)
		)
	}}</v-icon>
</template>

<script lang="ts">
	import Vue from 'vue'

	export default Vue.extend({
		name: 'WifiQuality',
		props: {
			quality: { type: Number }
		}
	})
</script>
