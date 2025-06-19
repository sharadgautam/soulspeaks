<template>
  <div class="home-bg"></div>
  <div class="home-container" :class="{ 'scroll-in': animate }">
    <h1>Welcome to SoulSpeaks Cloud PBX</h1>
    <p>Your modern, scalable, and reliable cloud phone system.</p>
    <router-link to="/login" class="login-link">Login to Your Account</router-link>
    <section class="sales">
      <h2>Why Choose SoulSpeaks?</h2>
      <ul>
        <li>Instantly buy and manage phone numbers (DIDs) worldwide</li>
        <li>Crystal-clear call quality powered by FreeSWITCH and Twilio</li>
        <li>Easy-to-use dashboard for all your business telephony needs</li>
        <li>24/7 support and enterprise-grade reliability</li>
        <li>Flexible plans for businesses of all sizes</li>
      </ul>
      <button class="cta" @click="goToSignup">Get Started Today</button>
    </section>
  </div>
</template>

<script>
export default {
  name: 'Home',
  data() {
    return {
      animate: false
    };
  },
  methods: {
    goToSignup() {
      this.$router.push('/signup');
    }
  },
  mounted() {
    setTimeout(() => {
      this.animate = true;
    }, 100); // slight delay for effect
  }
};
</script>

<style scoped>
.home-bg {
  position: fixed;
  top: 0; left: 0; right: 0; bottom: 0;
  width: 100vw;
  height: 100vh;
  z-index: 0;
  pointer-events: none;
}
.home-bg::before {
  content: '';
  position: absolute;
  top: 0; left: 0; right: 0; bottom: 0;
  width: 100vw;
  height: 100vh;
  background: linear-gradient(120deg, #b3e0ff 0%, #e0f7fa 100%);
  filter: blur(16px) brightness(1.1);
  z-index: 0;
}
.home-container {
  max-width: 700px;
  margin: 60px auto;
  padding: 2em;
  text-align: center;
  position: relative;
  z-index: 1;
  opacity: 0;
  transform: translateY(-60px);
  transition: none;
}
.home-container.scroll-in {
  animation: scrollDownFadeIn 1s cubic-bezier(0.23, 1, 0.32, 1) forwards;
}
@keyframes scrollDownFadeIn {
  0% {
    opacity: 0;
    transform: translateY(-60px);
  }
  100% {
    opacity: 1;
    transform: translateY(0);
  }
}
.login-link {
  display: inline-block;
  margin: 1.5em 0;
  padding: 0.75em 2em;
  background: #42b983;
  color: #fff;
  border-radius: 4px;
  text-decoration: none;
  font-weight: bold;
  font-size: 1.1em;
  transition: background 0.2s;
}
.login-link:hover {
  background: #369870;
}
.sales {
  margin-top: 2em;
  background: #f9f9f9;
  border-radius: 8px;
  padding: 2em 1em;
}
.sales h2 {
  margin-bottom: 1em;
}
.sales ul {
  text-align: left;
  margin: 0 auto 1.5em auto;
  max-width: 400px;
}
.sales li {
  margin-bottom: 0.7em;
  font-size: 1.1em;
}
.cta {
  background: #ff9800;
  color: #fff;
  border: none;
  border-radius: 4px;
  padding: 0.8em 2em;
  font-size: 1.1em;
  cursor: pointer;
  transition: background 0.2s;
}
.cta:hover {
  background: #e68900;
}
</style> 