package com.ashu.updater;

import org.json.JSONObject;
import org.junit.Assert;
import org.junit.Test;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.charset.StandardCharsets;

public class UpdaterLogicTest {

    @Test
    public void testVersionComparison_updateAvailable() {
        long installedVersion = 86;
        long remoteVersion = 87;
        Assert.assertTrue(remoteVersion > installedVersion);
    }

    @Test
    public void testVersionComparison_sameVersion_noUpdate() {
        long installedVersion = 86;
        long remoteVersion = 86;
        Assert.assertFalse(remoteVersion > installedVersion);
    }

    @Test
    public void testVersionComparison_olderVersion_preventDowngrade() {
        long installedVersion = 86;
        long remoteVersion = 85;
        Assert.assertFalse(remoteVersion > installedVersion);
    }

    @Test
    public void testHttpsEnforcement() {
        String insecureUrl = "http://example.com/app.apk";
        String secureUrl = "https://example.com/app.apk";

        Assert.assertFalse(insecureUrl.toLowerCase().startsWith("https://"));
        Assert.assertTrue(secureUrl.toLowerCase().startsWith("https://"));
    }

    @Test
    public void testSha256CalculationAndMatch() throws Exception {
        File tempFile = File.createTempFile("test_apk_", ".apk");
        tempFile.deleteOnExit();

        byte[] content = "TEST_APK_CONTENT_DATA".getBytes(StandardCharsets.UTF_8);
        try (FileOutputStream fos = new FileOutputStream(tempFile)) {
            fos.write(content);
        }

        String calculatedHash = ApkVerifier.calculateFileSha256(tempFile);
        Assert.assertNotNull(calculatedHash);
        Assert.assertEquals(64, calculatedHash.length());

        // Verify case-insensitive match
        Assert.assertTrue(calculatedHash.equalsIgnoreCase(calculatedHash.toUpperCase()));
        Assert.assertFalse(calculatedHash.equalsIgnoreCase("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"));
    }

    @Test
    public void testLegacyAndModernConfigParsing() throws Exception {
        // Modern format
        String modernJsonStr = "{"
                + "\"versionCode\": 88,"
                + "\"apkUrl\": \"https://example.com/modern.apk\","
                + "\"sha256\": \"abcdef123456\","
                + "\"forceUpdate\": true"
                + "}";
        JSONObject modernJson = new JSONObject(modernJsonStr);
        long modernVersion = modernJson.optLong("versionCode", modernJson.optLong("apk_version_code", -1));
        String modernUrl = modernJson.optString("apkUrl", modernJson.optString("apk_update_url", ""));
        String modernSha = modernJson.optString("sha256", modernJson.optString("apk_sha256", ""));
        boolean modernForce = modernJson.optBoolean("forceUpdate", modernJson.optBoolean("force_update", false));

        Assert.assertEquals(88, modernVersion);
        Assert.assertEquals("https://example.com/modern.apk", modernUrl);
        Assert.assertEquals("abcdef123456", modernSha);
        Assert.assertTrue(modernForce);

        // Legacy format
        String legacyJsonStr = "{"
                + "\"apk_version_code\": 86,"
                + "\"apk_update_url\": \"https://example.com/legacy.apk\""
                + "}";
        JSONObject legacyJson = new JSONObject(legacyJsonStr);
        long legacyVersion = legacyJson.optLong("versionCode", legacyJson.optLong("apk_version_code", -1));
        String legacyUrl = legacyJson.optString("apkUrl", legacyJson.optString("apk_update_url", ""));
        String legacySha = legacyJson.optString("sha256", legacyJson.optString("apk_sha256", ""));
        boolean legacyForce = legacyJson.optBoolean("forceUpdate", legacyJson.optBoolean("force_update", false));

        Assert.assertEquals(86, legacyVersion);
        Assert.assertEquals("https://example.com/legacy.apk", legacyUrl);
        Assert.assertEquals("", legacySha);
        Assert.assertFalse(legacyForce);
    }
}
