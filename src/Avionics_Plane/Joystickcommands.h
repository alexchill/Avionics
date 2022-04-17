#if !defined(__COMMAND_H__)
#define __COMMAND_H__

struct Commands
{
    byte roll;
    byte pitch;
    byte yaw;
    byte throttle;
};

//void defaultValue(Commands *cmd) {
//  cmd->roll = 90;
//  cmd->pitch = 90;
//  cmd->yaw = 90;
//  cmd->throttle = 0;
//}

String toString(Commands *cmd) {
  String str = "roll: ";
  str.concat(cmd->roll);
  str.concat(", pitch: ");
  str.concat(cmd->pitch);
  str.concat(", yaw: ");
  str.concat(cmd->yaw);
  str.concat(", throttle: ");
  str.concat(cmd->throttle);

  return str;
}

#endif // __COMMAND_H__