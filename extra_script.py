Import("env")
env.Replace(PROGNAME="ESP_Relay_HomeKit_v%s" % env.GetProjectOption("release_version"))