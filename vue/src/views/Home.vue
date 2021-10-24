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

				<v-col cols="12" md="6">

					<v-card>
						<v-card-title>
							Device Status
						</v-card-title>
						<v-card-text>
							<v-progress-circular v-if="loadingDevice" indeterminate color="primary" size="70" width="7" />
							<v-simple-table v-else>
								<tr><th>Memory free: </th><td>{{ device.memory_free }} oct</td></tr>
								<tr><th>Max free block: </th><td>{{ device.max_free_block }} oct</td></tr>
								<tr><th>Fragmentation: </th><td>{{ device.fragmentation }}%</td></tr>
							</v-simple-table>
						</v-card-text>

					</v-card>

				</v-col>

				<v-col cols="12" md="6">

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
			loadingDevice: true,
			interval: null,
			wifi: {
				mode: "",
				ip: "",
				quality: 0,
			},
			device: {
				fragmentation: 0,
				max_free_block: 0,
				memory_free: 0,
			}
		}),

		async mounted() {
			await Promise.all([
				this.refreshWifi(),
				this.refreshDevice(),
			]);
			this.interval = setInterval(() => this.refreshDevice(false), 2000) as any;

		},

		destroyed() {
			if (this.interval) {
				clearInterval(this.interval as any);
				this.interval = null;
			}
		},

		methods: {
			async refreshWifi() {
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
			async refreshDevice(loading: boolean = true) {
				this.error = "";
				this.loadingDevice = loading;
				try {
					this.device = await (await fetch('/api/status')).json();
				} catch(e) {
					this.error = "Error on load device status";
					console.error(e);
				}
				this.loadingDevice = false;
			},
		}
	})
</script>
