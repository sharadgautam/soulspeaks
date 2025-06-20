<template>
  <div class="sip-client-container">
    <h2>Cloud PBX Web SIP Client</h2>
    <form @submit.prevent="register">
      <input v-model="sipUri" placeholder="SIP URI (e.g. sip:1001@sip.example.com)" required />
      <input v-model="username" placeholder="Username" required />
      <input v-model="password" type="password" placeholder="Password" required />
      <input v-model="wsServer" placeholder="WebSocket Server (e.g. wss://sip.example.com:7443)" required />
      <button type="submit">Register</button>
    </form>
    <div v-if="registered" class="call-controls">
      <input v-model="destination" placeholder="Destination (e.g. 1002)" />
      <button @click="call">Call</button>
      <button @click="hangup" :disabled="!inCall">Hangup</button>
      <p>Call Status: {{ callStatus }}</p>
    </div>
    <p v-if="message" class="message">{{ message }}</p>
  </div>
</template>

<script>
import { UserAgent, Inviter, Registerer } from 'sip.js';

export default {
  name: 'SipClient',
  data() {
    return {
      sipUri: '',
      username: '',
      password: '',
      wsServer: '',
      userAgent: null,
      registerer: null,
      inviter: null,
      registered: false,
      destination: '',
      inCall: false,
      callStatus: 'Idle',
      message: ''
    };
  },
  methods: {
    async register() {
      this.message = '';
      try {
        if (this.userAgent) {
          await this.userAgent.stop();
        }
        this.userAgent = new UserAgent({
          uri: UserAgent.makeURI(this.sipUri),
          transportOptions: { server: this.wsServer },
          authorizationUsername: this.username,
          authorizationPassword: this.password
        });
        this.registerer = new Registerer(this.userAgent);
        await this.userAgent.start();
        await this.registerer.register();
        this.registered = true;
        this.message = 'Registered successfully!';
      } catch (e) {
        this.message = 'Registration failed: ' + (e.message || e);
      }
    },
    async call() {
      if (!this.destination) {
        this.message = 'Please enter a destination.';
        return;
      }
      try {
        this.inviter = new Inviter(this.userAgent, UserAgent.makeURI(`sip:${this.destination}@${this.sipUri.split('@')[1]}`));
        this.callStatus = 'Calling...';
        await this.inviter.invite();
        this.inCall = true;
        this.callStatus = 'In Call';
      } catch (e) {
        this.message = 'Call failed: ' + (e.message || e);
        this.callStatus = 'Idle';
      }
    },
    async hangup() {
      if (this.inviter) {
        await this.inviter.cancel();
        this.inCall = false;
        this.callStatus = 'Idle';
      }
    }
  }
};
</script>

<style scoped>
.sip-client-container {
  max-width: 500px;
  margin: 60px auto;
  padding: 2em;
  border: 1px solid #ccc;
  border-radius: 8px;
  text-align: center;
}
.call-controls {
  margin-top: 2em;
}
.message {
  color: green;
  margin-top: 1em;
}
</style> 