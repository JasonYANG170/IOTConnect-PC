# Protect against multiple inclusion, which would fail when already imported targets are
# added once more.
_qt_internal_check_multiple_inclusion(_targets_not_defined "Mqtt;MqttPrivate")

_qt_internal_create_versionless_targets("${_targets_not_defined}" Qt6)

unset(_targets_not_defined)
