<template>
	<v-card>

		<v-card-title>
			Remote Configuration
		</v-card-title>

		<v-card-text>

			<v-row>
				<v-col v-if="error" cols="12">
					<span class="error--text">{{ error }}</span>
				</v-col>
				<v-col v-if="success" cols="12">
					<span class="success--text">{{ success }}</span>
				</v-col>

				<v-col cols="12" md="6">

					<v-card>
						<v-card-title>
							Signal Saved
							<v-spacer />
							<v-btn color="secondary" small :loading="loadingSignals" @click="refreshSignals()" >Refresh</v-btn>
						</v-card-title>
						<v-card-text>
							
							<v-progress-circular v-if="loadingSignals" indeterminate color="primary" size="100" width="7" />
							<v-expansion-panels v-else multiple>
								<v-expansion-panel
									v-for="(signal, i) of signals"
									:key="i"
									@change="loadDetail(signal)"
								>
									<v-expansion-panel-header>
										{{ signal }}
										<v-spacer />
										<div style="margin-top: -10px; margin-bottom: -10px; text-align: right;">
											<v-btn
												color="secondary"
												icon
												@click.stop="sendSignals.push({ value: signal, repeat: 0, wait: 0 })"
											>
												<v-icon>{{ $icons.mdiPlus }}</v-icon>
											</v-btn>
											<v-btn
												color="primary"
												icon
												@click.stop="deleteSignal = signal"
											>
												<v-icon>{{ $icons.mdiDelete }}</v-icon>
											</v-btn>
										</div>
									</v-expansion-panel-header>
									<v-expansion-panel-content>

										<v-progress-circular v-if="!signalsDetails[signal]" indeterminate color="primary"  size="70" width="5" />
										<p class="error--text" v-else-if="signalsDetails[signal].error" >{{ signalsDetails[signal].error }}</p>

										<div v-else>
											<signal :signal="signalsDetails[signal]" />
										</div>

									</v-expansion-panel-content>
								</v-expansion-panel>
							</v-expansion-panels>

						</v-card-text>
					</v-card>
				</v-col>

				<v-col cols="12" md="6">

					<v-card>
						<v-card-title>
							Signal Receive History
							<v-spacer />
							<v-btn color="secondary" small :loading="loadingHistory" @click="refreshHistory()" >Refresh</v-btn>
						</v-card-title>
						<v-card-text>
							
							<v-progress-circular v-if="loadingHistory" indeterminate color="primary" size="100" width="7" />
							<div v-else>
								<v-expansion-panels v-model="panel" multiple>
									<v-expansion-panel
										v-for="(signal, i) of history"
										:key="i"
									>
										<v-expansion-panel-header ripple>
											{{ signal.id }}
											<v-spacer />
											<div style="margin-top: -10px; margin-bottom: -10px; text-align: right;">
												<v-btn
													color="primary"
													icon
													@click.stop="saveKey = ''; saveStore = signal"
												>
													<v-icon>{{ $icons.mdiContentSave }}</v-icon>
												</v-btn>
											</div>
										</v-expansion-panel-header>
										<v-expansion-panel-content>
											<div>
												<signal :signal="signal" />
											</div>

										</v-expansion-panel-content>
									</v-expansion-panel>
								</v-expansion-panels>
							</div>

						</v-card-text>
					</v-card>

				</v-col>

			</v-row>

			<v-row>
				<v-col cols="12">

					<v-card>
						<v-card-title>
							Send signal (Use <v-icon>{{ $icons.mdiPlus }}</v-icon> button)
						</v-card-title>
						<v-card-text>
							<v-simple-table>
								<tr>
									<th class="text-left" style="width: 20%;" >Signal key</th>
									<th class="text-left" style="width: 40%;" >Repeat duration</th>
									<th class="text-left" style="width: 40%;" >Wait duration</th>
								</tr>
								<tr v-for="(signal, key) of sendSignals" :key="key">
									<td class="vertical-align: middle">{{ signal.value }}</td>
									<td>
										<v-text-field
											v-model.number="signal.repeat"
											dense
											outlined
											hide-details
											type="number"
											min="0"
											step="10"
										/>
									</td>
									<td>
										<v-text-field
											v-model.number="signal.wait"
											dense
											outlined
											hide-details
											type="number"
											min="0"
											step="10"
										/>
									</td>
								</tr>
							</v-simple-table>
						</v-card-text>
						<v-divider />
						<v-card-actions>
							<v-spacer/>
							<v-btn color="success" @click="send()" :loading="sendLoading">Send signal</v-btn>
						</v-card-actions>
					</v-card>

				</v-col>
			</v-row>

			<v-row>
				<v-col cols="12">


					<v-card>
						<v-card-title>
							Global
						</v-card-title>
						<v-card-text>
							<a id="downloadAnchorElem" style="display:none"></a>
							<v-btn color="secondary" @click="download()" :loading="downloadLoading" >Download configuration</v-btn>
							
							<div class="mt-4">
								<v-file-input
									v-model="loadConfigFile"
									label="Load configuration"
									show-size
									outlined
									dense
								>
									<template #append>
										<v-btn
											style="margin-top: -2px;"
											color="primary"
											:loading="loadConfigLoading"
											:disabled="!loadConfigFile"
											small
											@click="loadConfig()"
										>
											Load
										</v-btn>
									</template>
								</v-file-input>
							</div>
						</v-card-text>
					</v-card>
				</v-col>
			</v-row>

		</v-card-text>
		

		<v-dialog :value="!!saveStore" @input="saveStore = null">
			<v-card>
				<v-card-title>
					Save signal data
				</v-card-title>
				<v-card-text>

					<signal :signal="saveStore" />
					<v-text-field
						ref="keyInput"
						v-model="saveKey"
						label="Key"
						class="mt-4"
						clearable
						outlined
						@change="matchKey"
						@keyup="matchKey"
						@keydown="matchKey"
						@blur="matchKey"
					/>

				</v-card-text>
				<v-card-actions>
					<v-spacer />
					<v-btn @click="saveStore = null" >Cancel</v-btn>
					<v-btn @click="save()" :loading="saveLoading" color="primary">Save</v-btn>
				</v-card-actions>
			</v-card>
		</v-dialog>
		
		<v-dialog :value="!!deleteSignal" @input="deleteSignal = null">
			<v-card>
				<v-card-title>
					Are you sure to delete stored signal with key "<strong>{{ deleteSignal}}</strong>" ?
				</v-card-title>
				<v-card-text>

					<v-progress-circular v-if="!signalsDetails[deleteSignal]" indeterminate color="primary"  size="70" width="5" />
					<p class="error--text" v-else-if="signalsDetails[deleteSignal].error" >{{ signalsDetails[deleteSignal].error }}</p>

					<div v-else>
						<signal :signal="signalsDetails[deleteSignal]" />
					</div>

				</v-card-text>
				<v-card-actions>
					<v-spacer />
					<v-btn @click="deleteSignal = null" >Cancel</v-btn>
					<v-btn @click="remove()" :loading="deleteLoading" color="primary">Delete</v-btn>
				</v-card-actions>
			</v-card>
		</v-dialog>
	</v-card>
</template>

<script lang="ts">
  import Vue from 'vue'
	import Signal from '../components/Signal.vue'
  
  export default Vue.extend({
    name: 'Remote',

	components: {
		Signal
	},

	data: () => ({
		error: "",
		success: "",
		signals: [],
		signalsDetails: {},
		loadingSignals: true,
		history: [],
		loadingHistory: true,
        panel: [],
		saveStore: null,
		saveKey: "",
		saveLoading: false,
		deleteSignal: null,
		deleteLoading: false,
		downloadLoading: false,
		loadConfigFile: null,
		loadConfigLoading: false,
		sendSignals: [],
		sendLoading: false,
	} as {
		error: string,
		success: string,
		signals: any[],
		signalsDetails: any,
		loadingSignals: boolean,
		history: any[],
		loadingHistory: boolean,
		panel: number[],
		saveStore: any,
		saveKey: string,
		saveLoading: boolean,
		deleteSignal: string;
		deleteLoading: boolean,
		downloadLoading: boolean,
		loadConfigFile: File,
		loadConfigLoading: boolean,
		sendSignals: {
			value: string,
			repeat: number,
			wait: number,
		}[],
		sendLoading: boolean,
	}),

	async mounted() {
		await Promise.all([
			this.refreshSignals(),
			this.refreshHistory(),
		]);
	},

	watch: {
		deleteSignal(value: string) {
			if (value) {
				this.loadDetail(value);
			}
		},
		saveKey() {
			this.matchKey();
		}
	},

	methods: {

		async matchKey() {
			const saveKey = this.saveKey.replace(/[^\w_\-]/g, '');
			if (this.saveKey !== saveKey) {
				this.saveKey = saveKey;
				(this.$refs['keyInput'] as any).lazyValue = this.saveKey;
			}
		},

		async refreshSignals() {
			this.loadingSignals = true;
				try {
					this.signals = await (await fetch('/api/ir/store')).json();
					this.signals.sort();
					this.error = "";
					this.success = "";
				} catch(e) {
					this.error = "Error on load list signals stored";
					this.success = '';
					console.error(e);
				}
				this.loadingSignals = false;
		},
		
		async loadDetail(key: string) {
			try {
				this.signalsDetails[key] = await (await fetch(`/api/ir/store?key=${key}`)).json();
			} catch(e) {
				this.signalsDetails[key] = { error: 'Error on load detail.' };
				console.error(e);
			}
			this.signalsDetails = { ...this.signalsDetails };
		},

		async refreshHistory() {
			this.loadingHistory = true;
				try {
					this.history = await (await fetch('/api/ir/history-receive')).json();
					this.history.sort((a: any, b: any) => b.id - a.id);
					this.error = "";
					this.success = "";
					this.panel = [ 0 ];
				} catch(e) {
					this.error = "Error on load history stored";
					this.success = '';
					console.error(e);
				}
				this.loadingHistory = false;
		},

		async save() {
			this.saveLoading = true;
			try {
				await (await fetch('/api/ir/store', {
					method: 'POST',
					body: JSON.stringify({
						id: this.saveStore.id,
					    key: this.saveKey,
					})
				})).json();
				if (this.signalsDetails[this.saveStore.id]) {
					delete this.signalsDetails[this.saveStore.id];
				}
				await this.refreshSignals();
				this.saveStore = null;
				this.error = '';
				this.success = 'Signal stored';
			} catch(e) {
				this.error = "Error on save stored signal";
				this.success = '';
				console.error(e);
			}
			this.saveLoading = false;
		},

		async remove() {
			this.deleteLoading = true;
			try {
				await (await fetch('/api/ir/delete?key='+this.deleteSignal, {
					method: 'POST',
				})).json();
				if (this.signalsDetails[this.deleteSignal]) {
					delete this.signalsDetails[this.deleteSignal];
				}
				await this.refreshSignals();
				this.error = '';
				this.success = 'Signal "'+this.deleteSignal+'" deleted';
				this.deleteSignal = null;
			} catch(e) {
				this.error = "Error on delete stored signal";
				console.error(e);
			}
			this.deleteLoading = false;
		},

		async download() {
			this.downloadLoading = true;
			try {
				await this.refreshSignals();
				const dump: any = {};
				for (const signal of this.signals) {
					await this.loadDetail(signal);
					dump[signal] = this.signalsDetails[signal];
				}

				const dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(dump, null, 4));
				const dlAnchorElem = document.getElementById('downloadAnchorElem');
				dlAnchorElem.setAttribute("href", dataStr);
				dlAnchorElem.setAttribute("download", "config_ir_remote.json");
				dlAnchorElem.click();
				
			} catch (e) {
				this.error = 'Error on download configuration';
				this.success = '';
				console.error(e);
			}
			this.downloadLoading = false;
		},

		async loadConfig() {
			this.loadConfigLoading = true;

			try {
				const content: string = await new Promise((resolve, reject) => {
					const reader = new FileReader();
					reader.readAsText(this.loadConfigFile);
					reader.onload = (event) => resolve(reader.result.toString());
					reader.onerror = (error) => reject(error);
				});
				const json = JSON.parse(content);

				for (const [key, signal] of Object.entries(json)) {
					const r = await fetch(`/api/ir/restore?key=${key}`, {
						method: 'POST',
						body: JSON.stringify(signal)
					});
					if (!r.ok) {
						throw r;
					}
				}
				await this.refreshSignals();
				this.error = '';
				this.success = 'Configuration restaured';

			} catch (e) {
				this.error = "Error on load configuration from file";
				this.success = '';
				console.error(e);
			}

			this.loadConfigLoading = false;
		},

		async send() {
			this.sendLoading = true;
			try {
				await (await fetch('/api/ir/send', {
					method: 'POST',
					body: JSON.stringify(this.sendSignals)
				})).json();
				this.error = '';
				this.success = 'Send signals list success';
			} catch(e) {
				this.error = "Error on send signals list";
				this.success = '';
				console.error(e);
			}
			this.sendLoading = false;
		},
	}

  })
</script>
