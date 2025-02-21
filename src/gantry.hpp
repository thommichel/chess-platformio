#include "motor.hpp"

namespace gnt {
    class Gantry {
        public:
            typedef enum
            {
                PAWN = 0,
                ROOK = 1,
                BISHOP = 2,
                KNIGHT = 3,
                QUEEN = 4,
                KING = 5
            } ChessPiece;
            
            Gantry();

            Gantry(mtr::Motor x_axis, mtr::Motor y_axis, mtr::Motor z_axis, uint8_t magnet_pin);

            void home();

            mtr::Motor get_x_axis();
            
            mtr::Motor get_y_axis();
            
            mtr::Motor get_z_axis();

        private:
            mtr::Motor m_x_axis;
            mtr::Motor m_y_axis;
            mtr::Motor m_z_axis;
            uint8_t m_magnet_pin;
            
            void toggle_magnet(bool enable);

            void move_to(long x_mm, long y_mm);

            void pick_place_piece(ChessPiece piece, bool pickup);
            
    };
}