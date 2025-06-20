<template>
  <PageLayout>
    <div class="sip-client-container">
      <h2>Cloud PBX WebPhone</h2>
      <object
        data="/Browser-Phone/Phone/index.html"
        type="text/html"
        style="width:100%;height:700px;border:none;min-height:600px;"
        aria-label="Siperb Browser Phone"
      >
        Your browser does not support embedding the webphone. <a href="/Browser-Phone/Phone/index.html" target="_blank">Open in a new tab</a>.
      </object>
      <p class="webphone-note">
        This webphone is powered by the open-source <a href="https://github.com/Siperb/Browser-Phone" target="_blank" rel="noopener">Siperb Browser-Phone</a> project.<br/>
        Configure your SIP/FreeSWITCH credentials in the phone UI.
      </p>
    </div>
  </PageLayout>
</template>

<script>
import PageLayout from '../components/PageLayout.vue';
export default {
  name: 'SipClient',
  components: { PageLayout }
};
</script>

<style scoped>
.sip-client-container {
  max-width: 900px;
  margin: 40px auto;
  padding: 2em;
  border: 1px solid #ccc;
  border-radius: 12px;
  text-align: center;
  background: #fff;
  box-shadow: 0 4px 24px rgba(60,60,60,0.08);
}
.webphone-note {
  margin-top: 1.5em;
  color: #555;
  font-size: 1.1em;
}
.webphone-note a {
  color: #5B2A9D;
  text-decoration: underline;
}
</style> 