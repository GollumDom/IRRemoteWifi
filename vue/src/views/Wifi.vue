<template>
	<v-card>

		<v-card-title>
			Wifi Configuration
		</v-card-title>

		<v-card-text>

			<v-row>
				<v-col v-if="error" cols="12">
					<span class="error--text">{{ error }}</span>
				</v-col>
				<v-col v-if="success" cols="12">
					<span class="success--text">{{ success }}</span>
				</v-col>

				<v-col cols="12" md="8" lg="9">

					<v-progress-circular v-if="loadingInfo" indeterminate color="primary" size="100" width="7" />

					<v-form v-model="valid" v-else @submit.prevent="send">

						<v-card>
							<v-card-text>

								<p class="d-flex mb-8">
									<strong class="mr-5">Mode:</strong>
									<v-radio-group v-model="value.mode" row>
										<v-radio value="STATION" label="Station" class="mr-4" />
										<v-radio value="AP" label="AP" /> 
									</v-radio-group>
								</p>

								<v-select v-if="value.mode == 'STATION'"
									v-model="value.dhcp"
									:items="[
										{ value: 'DHCP', text: 'DHCP' },
										{ value: 'DHCP_DNS', text: 'DHCP without DNS' },
										{ value: 'STATIC', text: 'Static' },
									]"
									label="IP Mode"
									outlined
								/>

								<v-text-field
									v-model="value.ssid"
									:rules="[
										$validator.required()	
									]"
									:clear-icon="$icons.mdiCloseCircle"
									label="SSID"
									clearable
									outlined
									validate-on-blur
								/>

								<v-text-field
									v-model="value.password"
									:type="showPassword ? 'text' : 'password'"
									:append-icon="showPassword ? $icons.mdiEyeOff : $icons.mdiEye"
									:clear-icon="$icons.mdiCloseCircle"
									@click:append="showPassword = !showPassword"
									label="Password"
									clearable
									outlined
									validate-on-blur
								/>

								<template v-if="value.mode == 'STATION'">
									<template v-if="value.dhcp == 'STATIC'">

										<v-text-field
											v-model="value.ip"
											:rules="[
												$validator.required(),
												$validator.ipv4(),
											]"
											label="IP"
											placeholder="0.0.0.0"
											clearable
											outlined
											validate-on-blur
											@keydown="validateIp"
											@keyup="validateIp"
										/>

										<v-text-field
											v-model="value.mask"
											:rules="[
												$validator.required(),
												$validator.ipv4(),
											]"
											label="Mask"
											placeholder="255.255.255.0"
											clearable
											outlined
											validate-on-blur
											@keydown="validateIp"
											@keyup="validateIp"
										/>

										<v-text-field
											v-model="value.gateway"
											:rules="[
												$validator.ipv4(),
											]"
											label="Gateway"
											placeholder="255.255.255.0"
											clearable
											outlined
											validate-on-blur
											@keydown="validateIp"
											@keyup="validateIp"
										/>

									</template>

									<template v-if="value.dhcp != 'DHCP'">

										<v-text-field
											v-model="value.dns1"
											:rules="[
												$validator.ipv4(),
											]"
											label="DNS 1"
											placeholder="0.0.0.0"
											clearable
											outlined
											validate-on-blur
											@keydown="validateIp"
											@keyup="validateIp"
										/>

										<v-text-field
											v-model="value.dns2"
											:rules="[
												$validator.ipv4(),
											]"
											label="DNS 2"
											placeholder="0.0.0.0"
											clearable
											outlined
											validate-on-blur
											@keydown="validateIp"
											@keyup="validateIp"
										/>
									</template>
								</template>

							<v-divider class="mb-4"/>

							<div style="text-align: right">
								<v-btn
									type="submit"
									color="primary"
									:disabled="!valid"
									:loading="loadingSend"
								>Save</v-btn>
							</div>

							</v-card-text>

						</v-card>

					</v-form>

				</v-col>
				<v-col cols="12" md="4" lg="3">
					<v-card>

						<v-card-title>
							Wifi list:
							<v-spacer />
							<v-btn color="secondary" small :loading="loadingScan" @click="refresh()" >Refresh</v-btn>
						</v-card-title>

						<v-card-text>
							<template v-for="wifi of scan" v-ripple>

								<v-card class="mb-3" v-ripple style="cursor: pointer;" @click="value.ssid = wifi.ssid">
									<v-card-title>
										{{ wifi.ssid }}
									</v-card-title>
									<v-card-text>

										<div>
											<wifi-quality :quality="wifi.quality" />&nbsp;Quality: {{ wifi.quality }}
										</div>

										<div>
											<v-icon>{{ $icons.mdiLock }}</v-icon>&nbsp;Encryption: {{ wifi.encryption_type }}
										</div>
										
										<div style="text-align: right">
											<v-btn color="primary" small >Select</v-btn>
										</div>
									</v-card-text>

								</v-card>

							</template>
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
		name: 'Wifi',

		components: {
			WifiQuality
		},

		data: () => ({
			loadingInfo: false,
			loadingScan: false,
			loadingSend: false,
			error: "",
			success: "",
			scan: [],
			showPassword: false,
			valid: false,
			value: {
				mode: "AP",
				dhcp: "dhcp",
				ssid: "",
				password: "",
				ip: "",
				mask: "",
				gateway: "",
				dns1: "",
				dns2: "",
			}
		}),

		async mounted() {
			this.error = "";
			this.success = "";
			this.refresh();
			this.loadingInfo = true;
			try {
				this.value = await (await fetch('/api/wifi/config')).json();
			} catch(e) {
				this.error = "Error on load current wifi configutation";
				console.error(e);
			}
			this.loadingInfo = false;
		},

		methods: {
			async refresh() {
				this.loadingScan = true;
				try {
					this.scan = await (await fetch('/api/wifi/scan')).json();
				} catch(e) {
					this.error = "Error on load scan wifi";
					console.error(e);
				}
				this.loadingScan = false;
			},

			async send() {
				this.error = "";
				this.success = "";
				this.loadingSend = true;
				try {
					this.value = await (await fetch('/api/wifi/config', {
						method: 'POST',
						body: JSON.stringify(this.value)
					}).then(r => { if (r.ok) return r; throw r; })).json();
				} catch(e) {
					this.error = "Error on save wifi configuration";
					console.error(e);
				}
				this.success = "Saved with success.";
				this.loadingSend = false;
			},

			validateIp() {
				this.value.ip      = this.value.ip     .replace(/[^\d\.]/g, '');
				this.value.mask    = this.value.mask   .replace(/[^\d\.]/g, '');
				this.value.gateway = this.value.gateway.replace(/[^\d\.]/g, '');
				this.value.dns1    = this.value.dns1   .replace(/[^\d\.]/g, '');
				this.value.dns2    = this.value.dns2   .replace(/[^\d\.]/g, '');				
			}
		}
	})
</script>
