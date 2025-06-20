<template>
  <PageLayout>
    <div class="add-trunk-container">
      <h2 class="gradient-text">Destination Trunks</h2>
      <ul class="trunk-list">
        <li v-for="(trunk, idx) in trunks" :key="idx" class="card glass trunk-card">
          <strong>{{ trunk.name }}</strong><br />
          <span class="trunk-label">SIP URI 1:</span> {{ trunk.sipUri1 }}<br />
          <span class="trunk-label">SIP URI 2:</span> {{ trunk.sipUri2 }}
        </li>
      </ul>
      <button class="fancy-btn add-btn" @click="showModal = true">Add New Trunk</button>
      <div v-if="showModal" class="modal-bg" @click.self="showModal = false">
        <div class="modal glass">
          <h3 class="gradient-text">Add New Trunk</h3>
          <form @submit.prevent="addTrunk">
            <input v-model="newTrunkName" class="fancy-input" placeholder="Trunk Name" required />
            <input v-model="newSipUri1" class="fancy-input" placeholder="SIP URI 1 (e.g. sip:trunk1@sip.example.com)" required />
            <input v-model="newSipUri2" class="fancy-input" placeholder="SIP URI 2 (e.g. sip:trunk2@sip.example.com)" required />
            <div class="modal-actions">
              <button type="button" class="fancy-btn" @click="showModal = false">Cancel</button>
              <button type="submit" class="fancy-btn">Add Trunk</button>
            </div>
          </form>
          <p v-if="message" class="message">{{ message }}</p>
        </div>
      </div>
    </div>
  </PageLayout>
</template>

<script>
import PageLayout from '../components/PageLayout.vue';
export default {
  name: 'AddTrunk',
  components: { PageLayout },
  data() {
    return {
      newTrunkName: '',
      newSipUri1: '',
      newSipUri2: '',
      trunks: [],
      message: '',
      showModal: false
    };
  },
  methods: {
    addTrunk() {
      if (
        this.newTrunkName &&
        this.newSipUri1 &&
        this.newSipUri2 &&
        !this.trunks.some(t => t.name === this.newTrunkName)
      ) {
        this.trunks.push({
          name: this.newTrunkName,
          sipUri1: this.newSipUri1,
          sipUri2: this.newSipUri2
        });
        this.message = `Trunk added: ${this.newTrunkName}`;
        this.newTrunkName = '';
        this.newSipUri1 = '';
        this.newSipUri2 = '';
        this.showModal = false;
      } else {
        this.message = 'Please fill all fields and use a unique trunk name.';
      }
    }
  }
};
</script>

<style scoped>
.add-trunk-container {
  max-width: 600px;
  margin: 60px auto;
  padding: 2.5em 1.5em;
  border-radius: 16px;
  text-align: center;
  background: none;
}
.trunk-list {
  list-style: none;
  padding: 0;
  margin: 0 0 2em 0;
  display: flex;
  flex-direction: column;
  gap: 1.2em;
}
.trunk-card {
  padding: 1.2em 1em;
  border-radius: 14px;
  font-size: 1.1em;
  background: rgba(255,255,255,0.85);
  border: 1.5px solid var(--primary, #5B2A9D);
  box-shadow: 0 2px 12px rgba(60,60,60,0.08);
  transition: box-shadow 0.2s, border 0.2s, transform 0.13s;
}
.trunk-card:hover {
  box-shadow: 0 8px 32px rgba(60,60,60,0.18);
  border-color: var(--accent, #00BFAE);
  transform: translateY(-4px) scale(1.03);
}
.trunk-label {
  color: var(--primary, #5B2A9D);
  font-weight: 700;
}
.add-btn {
  margin-top: 1.5em;
  font-size: 1.1em;
}
.modal-bg {
  position: fixed;
  inset: 0;
  background: rgba(40,30,60,0.25);
  z-index: 1000;
  display: flex;
  align-items: center;
  justify-content: center;
}
.modal {
  min-width: 400px;
  max-width: 98vw;
  padding: 2.5em 2em 2em 2em;
  border-radius: 22px;
  box-shadow: 0 8px 32px rgba(60,60,60,0.18);
  position: relative;
  text-align: center;
  font-size: 0.97em;
}
.fancy-input {
  background: #fff;
  color: #222;
  border: 2px solid var(--primary, #5B2A9D);
  border-radius: 8px;
  padding: 0.6em 0.9em;
  font-size: 1em;
  font-family: inherit;
  box-shadow: 0 2px 8px rgba(60,60,60,0.07);
  transition: border 0.2s, box-shadow 0.2s;
  outline: none;
  margin: 0.5em auto;
  width: 100%;
  max-width: 400px;
  display: block;
  text-align: center;
}
.fancy-input::placeholder {
  text-align: center;
}
.fancy-input:focus {
  border-color: var(--accent, #00BFAE);
  box-shadow: 0 4px 16px rgba(0,191,174,0.10);
}
.fancy-input:hover {
  border-color: var(--primary-light, #7C4DFF);
}
.modal-actions {
  display: flex;
  gap: 1em;
  justify-content: center;
  margin-top: 1.2em;
  font-size: 0.97em;
}
.message {
  color: var(--accent, #00BFAE);
  margin-top: 1em;
  font-weight: 600;
}
@media (max-width: 700px) {
  .add-trunk-container {
    padding: 1.2em 0.2em;
    max-width: 98vw;
  }
  .modal {
    min-width: 0;
    padding: 1.2em 0.5em 1em 0.5em;
    font-size: 0.95em;
  }
}
.fancy-btn {
  min-width: 140px;
  font-size: 1.1em;
  font-weight: 700;
  border-radius: 8px;
  border: none;
  background: linear-gradient(90deg, #5B2A9D 0%, #00BFAE 100%);
  color: #fff;
  box-shadow: 0 4px 16px rgba(91,42,157,0.13);
  cursor: pointer;
  outline: none;
  padding: 0.8em 2em;
  position: relative;
  overflow: hidden;
  transition: background 0.2s, color 0.2s, box-shadow 0.2s, transform 0.13s;
}
.fancy-btn::after {
  content: '';
  position: absolute;
  left: -50%;
  top: -50%;
  width: 200%;
  height: 200%;
  background: linear-gradient(120deg, #00BFAE 0%, #7C4DFF 100%);
  opacity: 0;
  transition: opacity 0.3s;
  z-index: 0;
}
.fancy-btn:hover {
  background: linear-gradient(90deg, #00BFAE 0%, #7C4DFF 100%);
  color: #fff;
  box-shadow: 0 8px 32px rgba(91,42,157,0.18);
  transform: translateY(-2px) scale(1.04);
}
.fancy-btn:active {
  transform: scale(0.98);
}
</style> 