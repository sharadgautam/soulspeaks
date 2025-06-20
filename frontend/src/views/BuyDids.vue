<template>
  <PageLayout>
    <div class="buy-dids-container">
      <h2 class="gradient-text">Buy DIDs</h2>
      <form class="search-form glass" @submit.prevent="searchDids">
        <label>Country:</label>
        <select v-model="country">
          <option value="US">United States</option>
        </select>
        <label>Type:</label>
        <select v-model="didType">
          <option value="local">Geo (Local)</option>
          <option value="tollfree">Toll-Free</option>
        </select>
        <button type="submit" class="fancy-btn search-btn">Search DIDs</button>
      </form>
      <div v-if="dids.length">
        <h3 class="section-title">Available DIDs</h3>
        <div class="did-list">
          <div v-for="did in uniqueDids" :key="did.phone_number" class="card did-card">
            <span class="did-number">{{ formatNational(did.phone_number) }}</span>
            <button @click="buyDid(did.phone_number)" class="nav-btn buy-btn">Buy</button>
          </div>
        </div>
      </div>
      <p v-if="message" class="message">{{ message }}</p>
    </div>
  </PageLayout>
</template>

<script>
import axios from 'axios';
import { TWILIO_ACCOUNT_SID, TWILIO_AUTH_TOKEN } from '../twilio.config';
import PageLayout from '../components/PageLayout.vue';

export default {
  name: 'BuyDids',
  components: { PageLayout },
  data() {
    return {
      country: 'US',
      didType: 'local',
      dids: [],
      message: ''
    };
  },
  computed: {
    uniqueDids() {
      // Remove duplicates by phone_number
      const seen = new Set();
      return this.dids.filter(did => {
        if (seen.has(did.phone_number)) return false;
        seen.add(did.phone_number);
        return true;
      });
    }
  },
  methods: {
    formatNational(e164) {
      // Only for US numbers, format as (NPA) NXX-XXXX
      if (!e164.startsWith('+1') || e164.length !== 12) return e164;
      const npa = e164.slice(2, 5);
      const nxx = e164.slice(5, 8);
      const xxxx = e164.slice(8);
      return `(${npa}) ${nxx}-${xxxx}`;
    },
    async searchDids() {
      this.message = '';
      this.dids = [];
      const typePath = this.didType === 'tollfree' ? 'TollFree' : 'Local';
      const url = `https://api.twilio.com/2010-04-01/Accounts/${TWILIO_ACCOUNT_SID}/AvailablePhoneNumbers/${this.country}/${typePath}.json?SmsEnabled=true&VoiceEnabled=true&Limit=10`;
      try {
        const res = await axios.get(url, {
          auth: {
            username: TWILIO_ACCOUNT_SID,
            password: TWILIO_AUTH_TOKEN
          }
        });
        this.dids = res.data.available_phone_numbers || [];
        if (!this.dids.length) this.message = 'No DIDs found.';
      } catch (e) {
        this.message = 'Failed to search DIDs from Twilio.';
      }
    },
    async buyDid(phoneNumber) {
      this.message = '';
      const url = `https://api.twilio.com/2010-04-01/Accounts/${TWILIO_ACCOUNT_SID}/IncomingPhoneNumbers.json`;
      try {
        const res = await axios.post(url, `PhoneNumber=${encodeURIComponent(phoneNumber)}`, {
          auth: {
            username: TWILIO_ACCOUNT_SID,
            password: TWILIO_AUTH_TOKEN
          },
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' }
        });
        if (res.data.sid) {
          this.message = `Successfully bought DID: ${res.data.phone_number}`;
        } else {
          this.message = 'Failed to buy DID.';
        }
      } catch (e) {
        this.message = 'Failed to buy DID from Twilio.';
      }
    }
  }
};
</script>

<style scoped>
.buy-dids-container {
  max-width: 700px;
  margin: 60px auto;
  padding: 2.5em 1.5em;
  border-radius: 16px;
  text-align: center;
  background: none;
}
.search-form {
  display: flex;
  flex-wrap: wrap;
  gap: 1em 2em;
  align-items: center;
  justify-content: center;
  margin-bottom: 2em;
  padding: 1.5em 1em;
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
.section-title {
  margin-bottom: 1.2em;
  font-size: 1.3em;
  font-weight: 700;
  color: var(--primary, #5B2A9D);
}
.did-list {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 1.5em;
  margin-top: 1em;
}
.did-card {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1em;
  padding: 1.5em 1em;
  border-radius: 14px;
  background: rgba(255,255,255,0.85);
  box-shadow: 0 2px 12px rgba(60,60,60,0.08);
  border: 1.5px solid var(--primary, #5B2A9D);
  transition: box-shadow 0.2s, border 0.2s, transform 0.13s;
}
.did-card:hover {
  box-shadow: 0 8px 32px rgba(60,60,60,0.18);
  border-color: var(--accent, #00BFAE);
  transform: translateY(-4px) scale(1.03);
}
.did-number {
  font-size: 1.3em;
  font-weight: 800;
  color: var(--primary, #5B2A9D);
  letter-spacing: 1px;
}
.buy-btn {
  min-width: 100px;
  font-size: 1.1em;
  font-weight: 700;
  border-radius: 8px;
  border: 2px solid var(--primary, #5B2A9D);
  background: #fff;
  color: var(--primary, #5B2A9D);
  box-shadow: 0 2px 8px rgba(60,60,60,0.07);
  transition: background 0.2s, color 0.2s, border 0.2s, box-shadow 0.2s, transform 0.1s;
  cursor: pointer;
  outline: none;
  padding: 0.7em 1.2em;
  margin-top: 0.5em;
}
.buy-btn:hover {
  background: var(--primary, #5B2A9D);
  color: #fff;
  border-color: var(--accent, #00BFAE);
  box-shadow: 0 6px 24px rgba(60,60,60,0.13);
  transform: translateY(-2px) scale(1.03);
}
.message {
  color: var(--accent, #00BFAE);
  margin-top: 1em;
  font-weight: 600;
}
@media (max-width: 700px) {
  .buy-dids-container {
    padding: 1.2em 0.2em;
    max-width: 98vw;
  }
  .search-form {
    flex-direction: column;
    gap: 0.7em;
    padding: 1em 0.2em;
  }
  .did-list {
    grid-template-columns: 1fr;
    gap: 1em;
  }
}
</style> 