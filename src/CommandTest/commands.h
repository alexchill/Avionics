#if !defined(__COMMAND_H__)
#define __COMMAND_H__

// defines commands used for plane

// Define command names
#define CMD_NOP 0
#define CMD_ROLL 1
#define CMD_PITCH 2
#define CMD_YAW 3
#define CMD_PITCH_TRIM 4
#define CMD_NAVLED 5

struct Command
{
    // fields
    uint8_t op;  // up to 256 command ops
    int16_t val; // val range is 32,768 to 32,767

    // constructors
    Command() {}
    Command(uint8_t new_op, int16_t new_val) : op(new_op), val(new_val) {}

    String toString()
    {
        String str = "Cmd: ";

        switch (op)
        {
            case CMD_NOP:
                str.concat("NOP");
                break;
            case CMD_ROLL:
                str.concat("ROLL");
                break;
            case CMD_PITCH:
                str.concat("PITCH");
                break;
            case CMD_YAW:
                str.concat("YAW");
                break;
            case CMD_PITCH_TRIM:
                str.concat("PITCH_TRIM");
                break;
            case CMD_NAVLED:
                str.concat("NAVLED");
                break;

            default:
                str.concat("Unrecognized command: ");
                str.concat(op);
        }

        str.concat(", val: ");
        str.concat(val);

        return str;
    }
};

#endif // __COMMAND_H__