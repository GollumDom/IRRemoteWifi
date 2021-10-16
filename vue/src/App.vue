<template>
	<v-app>
		<v-app-bar
			app
			color="primary"
			dark
		>
			<v-spacer/>
			<v-toolbar-title>ESP 8266 - Remote Control - Admin</v-toolbar-title>
			<v-spacer/>

			<template #extension>
				<v-tabs fixed-tabs>
					<v-tab :to="{ name: 'home' }"><v-icon>{{ $icons.mdiHome }}</v-icon>&nbsp;Home</v-tab>
					<v-tab :to="{ name: 'wifi' }"><v-icon>{{ $icons.mdiWifi }}</v-icon>&nbsp;Wifi</v-tab>
					<v-tab :to="{ name: 'remote' }"><v-icon>{{ $icons.mdiRemote }}</v-icon>&nbsp;Remote</v-tab>
				</v-tabs>
			</template>
		</v-app-bar>

		<v-main>
			<v-container>
				<router-view/>
			</v-container>
		</v-main>
	</v-app>
</template>

<script lang="ts">
import Vue from 'vue';

export default Vue.extend({
	name: 'App',
});
</script>
