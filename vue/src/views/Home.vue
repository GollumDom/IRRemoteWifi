<template>
	<v-card>

		<v-card-title>
			ESP 8266 - Remote Control - Status
		</v-card-title>

		<v-card-text>
			
			<v-row>
				<v-col v-if="error" cols="12">
					<span class="error--text">{{ error }}</span>
				</v-col>

				<v-col cols="12" md="4">

					<v-card>
						<v-card-title>
							Wifi Status
						</v-card-title>
						<v-card-text>
							<v-progress-circular v-if="loadingWifi" indeterminate color="primary" size="70" width="7" />
							<v-simple-table v-else>
								<tr><th>Mode: </th><td>{{ wifi.mode }}</td></tr>
								<tr><th>IP: </th><td>{{ wifi.ip }}</td></tr>
								<tr><th>Quality: </th><td><wifi-quality :quality="wifi.quality" />&nbsp;{{ wifi.quality }}</td></tr>
							</v-simple-table>
						</v-card-text>

					</v-card>

				</v-col>
			</v-row>

		</v-card-text>

  </v-card>
</template>

<script lang="ts">
	import Vue from 'vue'
	import WifiQuality from '../components/WifiQuality.vue'

	export default Vue.extend({
		name: 'Home',

		components: {
			WifiQuality
		},

		data: () => ({
			error: "",
			loadingWifi: true,
			wifi: {
				mode: "",
				ip: "",
				quality: 0,
			}
		}),

		async mounted() {
			this.error = "";
			this.loadingWifi = true;
			try {
				this.wifi = await (await fetch('/api/wifi/status')).json();
			} catch(e) {
				this.error = "Error on load wifi status";
				console.error(e);
			}
			this.loadingWifi = false;
		},
	})
</script>
