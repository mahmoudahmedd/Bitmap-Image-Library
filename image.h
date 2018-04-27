/*
****************************************************************************
*                        FCI � Programming 2 � 2018                        *
** Project Name: Bitmap Image Library                                      *
** Last Modification Date: 18/04/2018                                      *
** Author: Mahmoud Ahmed Tawfik                                            *
** Note: This library only supports 24-bits per pixel bitmap format files. *
****************************************************************************
*/

#ifndef __H__IMAGE__
#define __H__IMAGE__

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

class image
{
public:
    enum channel_mode { rgb_mode = 0, bgr_mode = 1};
    struct rgb_t
    {
        unsigned char   red;
        unsigned char green;
        unsigned char  blue;
    };
    image()
        :
        file_name_(""),
        width_(0),
        height_(0),
        row_increment_(0),
        bytes_per_pixel_(3)
    {}
    image(const std::string& filename)
        :
        file_name_(filename),
        width_          (0),
        height_         (0),
        row_increment_  (0),
        bytes_per_pixel_(0),
        channel_mode_(bgr_mode)
    {
        load_bitmap();
    }

    void save_image(const std::string& file_name) const
    {
        std::ofstream stream(file_name.c_str(),std::ios::binary);

        if (!stream)
        {
            std::cerr << "bitmap_image::save_image(): Error - Could not open file "  << file_name << " for writing!" << std::endl;
            return;
        }

        bitmap_information_header bih;

        bih.width            = width_;
        bih.height           = height_;
        bih.bit_count        = static_cast<unsigned short>(bytes_per_pixel_ << 3);
        bih.clr_important    = 0;
        bih.clr_used         = 0;
        bih.compression      = 0;
        bih.planes           = 1;
        bih.size             = bih.struct_size();
        bih.x_pels_per_meter = 0;
        bih.y_pels_per_meter = 0;
        bih.size_image       = (((bih.width * bytes_per_pixel_) + 3) & 0x0000FFFC) * bih.height;

        bitmap_file_header bfh;

        bfh.type             = 19778;
        bfh.size             = bfh.struct_size() + bih.struct_size() + bih.size_image;
        bfh.reserved1        = 0;
        bfh.reserved2        = 0;
        bfh.off_bits         = bih.struct_size() + bfh.struct_size();

        write_bfh(stream,bfh);
        write_bih(stream,bih);

        unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
        char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

        for (unsigned int i = 0; i < height_; ++i)
        {
            const unsigned char* data_ptr = &data_[(row_increment_ * (height_ - i - 1))];

            stream.write(reinterpret_cast<const char*>(data_ptr), sizeof(unsigned char) * bytes_per_pixel_ * width_);
            stream.write(padding_data,padding);
        }

        stream.close();
    }


    void get_pixel(const unsigned int x, const unsigned int y,
                   unsigned char& red,
                   unsigned char& green,
                   unsigned char& blue) const
    {
        const unsigned int y_offset = y * row_increment_;
        const unsigned int x_offset = x * bytes_per_pixel_;
        const unsigned int offset   = y_offset + x_offset;

        blue  = data_[offset + 0];
        green = data_[offset + 1];
        red   = data_[offset + 2];
    }
    template <typename RGB>
    void get_pixel(const unsigned int x, const unsigned int y, RGB& colour) const
    {
        get_pixel(x, y, colour.red, colour.green, colour.blue);
    }
    rgb_t get_pixel(const unsigned int x, const unsigned int y) const
    {
        rgb_t colour;
        get_pixel(x, y, colour.red, colour.green, colour.blue);
        return colour;
    }
	void set_pixel(const unsigned int x, const unsigned int y,
                   const unsigned char red,
                   const unsigned char green,
                   const unsigned char blue)
    {
        const unsigned int y_offset = y * row_increment_;
        const unsigned int x_offset = x * bytes_per_pixel_;
        const unsigned int offset   = y_offset + x_offset;

        data_[offset + 0] = blue;
        data_[offset + 1] = green;
        data_[offset + 2] = red;
    }
	void set_pixel(const unsigned int x, const unsigned int y,rgb_t _color)
    {
		set_pixel(x,y,_color.red,_color.green,_color.blue);
    }
	void invert_image()
	{
		for(unsigned int i = 0 ;i < this->height_;i++)
		{
			rgb_t color;
			for(unsigned int j = 0 ;j < this->width_;j++)
			{
				this->get_pixel(i,j,color);
				this->set_pixel(i,j,255-color.red,255-color.red,255-color.blue);
			}
		}
	}
	void lighten_image(double lighten)
	{
		for(unsigned int i = 0 ;i < this->height_;i++)
		{
			rgb_t color;
			for(unsigned int j = 0 ;j < this->width_;j++)
			{
				this->get_pixel(i,j,color);
				this->set_pixel(i,j,min(int(color.red+lighten*255),255),min(int(color.green+lighten*255),255),min(int(color.blue+lighten*255),255));
			}
		}
	}
	void darken_image(double lighten)
	{
		for(unsigned int i = 0 ;i < this->height_;i++)
		{
			rgb_t color;
			for(unsigned int j = 0 ;j < this->width_;j++)
			{
				this->get_pixel(i,j,color);
				this->set_pixel(i,j,max(int(color.red-lighten*255),0),max(int(color.green-lighten*255),0),max(int(color.blue-lighten*255),0));
			}
		}
	}
    unsigned int pixel_count() const
    {
        return width_ *  height_;
    }


private:

    std::string  file_name_;
    unsigned int width_;
    unsigned int height_;
    unsigned int row_increment_;
    unsigned int bytes_per_pixel_;
    channel_mode channel_mode_;
    std::vector<unsigned char> data_;

    struct bitmap_file_header
    {
        unsigned short type;
        unsigned int   size;
        unsigned short reserved1;
        unsigned short reserved2;
        unsigned int   off_bits;

        unsigned int struct_size() const
        {
            return sizeof(type     ) +
                   sizeof(size     ) +
                   sizeof(reserved1) +
                   sizeof(reserved2) +
                   sizeof(off_bits ) ;
        }
        void clear()
        {
            std::memset(this, 0x00, sizeof(bitmap_file_header));
        }
    };

    struct bitmap_information_header
    {
        unsigned int   size;
        unsigned int   width;
        unsigned int   height;
        unsigned short planes;
        unsigned short bit_count;
        unsigned int   compression;
        unsigned int   size_image;
        unsigned int   x_pels_per_meter;
        unsigned int   y_pels_per_meter;
        unsigned int   clr_used;
        unsigned int   clr_important;

        unsigned int struct_size() const
        {
            return sizeof(size            ) +
                   sizeof(width           ) +
                   sizeof(height          ) +
                   sizeof(planes          ) +
                   sizeof(bit_count       ) +
                   sizeof(compression     ) +
                   sizeof(size_image      ) +
                   sizeof(x_pels_per_meter) +
                   sizeof(y_pels_per_meter) +
                   sizeof(clr_used        ) +
                   sizeof(clr_important   ) ;
        }

        void clear()
        {
            std::memset(this, 0x00, sizeof(bitmap_information_header));
        }
    };
    void write_bfh(std::ofstream& stream, const bitmap_file_header& bfh) const
    {
        if (big_endian())
        {
            write_to_stream(stream,flip(bfh.type     ));
            write_to_stream(stream,flip(bfh.size     ));
            write_to_stream(stream,flip(bfh.reserved1));
            write_to_stream(stream,flip(bfh.reserved2));
            write_to_stream(stream,flip(bfh.off_bits ));
        }
        else
        {
            write_to_stream(stream,bfh.type     );
            write_to_stream(stream,bfh.size     );
            write_to_stream(stream,bfh.reserved1);
            write_to_stream(stream,bfh.reserved2);
            write_to_stream(stream,bfh.off_bits );
        }
    }
    template <typename T>
    void write_to_stream(std::ofstream& stream,const T& t) const
    {
        stream.write(reinterpret_cast<const char*>(&t),sizeof(T));
    }
    void write_bih(std::ofstream& stream, const bitmap_information_header& bih) const
    {
        if (big_endian())
        {
            write_to_stream(stream,flip(bih.size            ));
            write_to_stream(stream,flip(bih.width           ));
            write_to_stream(stream,flip(bih.height          ));
            write_to_stream(stream,flip(bih.planes          ));
            write_to_stream(stream,flip(bih.bit_count       ));
            write_to_stream(stream,flip(bih.compression     ));
            write_to_stream(stream,flip(bih.size_image      ));
            write_to_stream(stream,flip(bih.x_pels_per_meter));
            write_to_stream(stream,flip(bih.y_pels_per_meter));
            write_to_stream(stream,flip(bih.clr_used        ));
            write_to_stream(stream,flip(bih.clr_important   ));
        }
        else
        {
            write_to_stream(stream,bih.size            );
            write_to_stream(stream,bih.width           );
            write_to_stream(stream,bih.height          );
            write_to_stream(stream,bih.planes          );
            write_to_stream(stream,bih.bit_count       );
            write_to_stream(stream,bih.compression     );
            write_to_stream(stream,bih.size_image      );
            write_to_stream(stream,bih.x_pels_per_meter);
            write_to_stream(stream,bih.y_pels_per_meter);
            write_to_stream(stream,bih.clr_used        );
            write_to_stream(stream,bih.clr_important   );
        }
    }
    void read_bih(std::ifstream& stream,bitmap_information_header& bih)
    {
        read_from_stream(stream,bih.size            );
        read_from_stream(stream,bih.width           );
        read_from_stream(stream,bih.height          );
        read_from_stream(stream,bih.planes          );
        read_from_stream(stream,bih.bit_count       );
        read_from_stream(stream,bih.compression     );
        read_from_stream(stream,bih.size_image      );
        read_from_stream(stream,bih.x_pels_per_meter);
        read_from_stream(stream,bih.y_pels_per_meter);
        read_from_stream(stream,bih.clr_used        );
        read_from_stream(stream,bih.clr_important   );

        if (big_endian())
        {
            bih.size          = flip(bih.size               );
            bih.width         = flip(bih.width              );
            bih.height        = flip(bih.height             );
            bih.planes        = flip(bih.planes             );
            bih.bit_count     = flip(bih.bit_count          );
            bih.compression   = flip(bih.compression        );
            bih.size_image    = flip(bih.size_image         );
            bih.x_pels_per_meter = flip(bih.x_pels_per_meter);
            bih.y_pels_per_meter = flip(bih.y_pels_per_meter);
            bih.clr_used      = flip(bih.clr_used           );
            bih.clr_important = flip(bih.clr_important      );
        }
    }
    bool big_endian() const
    {
        unsigned int v = 0x01;

        return (1 != reinterpret_cast<char*>(&v)[0]);
    }
    unsigned short flip(const unsigned short& v) const
    {
        return ((v >> 8) | (v << 8));
    }

    unsigned int flip(const unsigned int& v) const
    {
        return (
                   ((v & 0xFF000000) >> 0x18) |
                   ((v & 0x000000FF) << 0x18) |
                   ((v & 0x00FF0000) >> 0x08) |
                   ((v & 0x0000FF00) << 0x08)
               );
    }
    void load_bitmap()
    {
        std::ifstream stream(file_name_.c_str(),std::ios::binary);

        if (!stream)
        {
            std::cerr << "image::load_bitmap() ERROR: bitmap_image - file " << file_name_ << " not found!" << std::endl;
            return;
        }

        width_  = 0;
        height_ = 0;

        bitmap_file_header bfh;
        bitmap_information_header bih;
		
        bfh.clear();
        bih.clear();

        read_bfh(stream,bfh);
        read_bih(stream,bih);

        if (bfh.type != 19778)
        {
            bfh.clear();
            bih.clear();

            stream.close();

            std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid type value " << bfh.type << " expected 19778." << std::endl;
            return;
        }

        if (bih.bit_count != 24)
        {
            bfh.clear();
            bih.clear();

            stream.close();

            std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid bit depth " << bih.bit_count << " expected 24." << std::endl;

            return;
        }

        if (bih.size != bih.struct_size())
        {
            bfh.clear();
            bih.clear();

            stream.close();

            std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Invalid BIH size " << bih.size << " expected " << bih.struct_size() << std::endl;

            return;
        }

        width_  = bih.width;
        height_ = bih.height;

        bytes_per_pixel_ = bih.bit_count >> 3;

        unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
        char padding_data[4] = {0,0,0,0};

        std::size_t bitmap_file_size = file_size(file_name_);

        std::size_t bitmap_logical_size = (height_ * width_ * bytes_per_pixel_) +
                                          (height_ * padding)                   +
                                          bih.struct_size()                    +
                                          bfh.struct_size()                    ;

        if (bitmap_file_size != bitmap_logical_size)
        {
            bfh.clear();
            bih.clear();

            stream.close();

            std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - Mismatch between logical and physical sizes of bitmap. " <<
                      "Logical: "  << bitmap_logical_size << " " <<
                      "Physical: " << bitmap_file_size    << std::endl;

            return;
        }

        create_bitmap();

        for (unsigned int i = 0; i < height_; ++i)
        {
            unsigned char* data_ptr = row(height_ - i - 1); // read in inverted row order

            stream.read(reinterpret_cast<char*>(data_ptr), sizeof(char) * bytes_per_pixel_ * width_);
            stream.read(padding_data,padding);
        }
		cout<<bfh.type<<endl;
		cout<<bfh.size<<endl;
		cout<<bfh.off_bits<<endl;
		cout<<bfh.reserved1<<endl;
		cout<<bfh.reserved2<<endl;
		cout<<bih.bit_count<<endl;
		cout<<bih.clr_important<<endl;
		cout<<bih.clr_used<<endl;
		cout<<bih.compression<<endl;
		cout<<bih.height<<endl;
		cout<<bih.planes<<endl;
		cout<<bih.size_image<<endl;
		cout<<bih.width<<endl;
		cout<<bih.x_pels_per_meter<<endl;
    }
    void create_bitmap()
    {
        row_increment_ = width_ * bytes_per_pixel_;
        data_.resize(height_ * row_increment_);
    }
    void read_bfh(std::ifstream& stream, bitmap_file_header& bfh)
    {
        read_from_stream(stream,bfh.type     );
        read_from_stream(stream,bfh.size     );
        read_from_stream(stream,bfh.reserved1);
        read_from_stream(stream,bfh.reserved2);
        read_from_stream(stream,bfh.off_bits );

        if (big_endian())
        {
            bfh.type      = flip(bfh.type     );
            bfh.size      = flip(bfh.size     );
            bfh.reserved1 = flip(bfh.reserved1);
            bfh.reserved2 = flip(bfh.reserved2);
            bfh.off_bits  = flip(bfh.off_bits );
        }
    }

    template <typename T>
    void read_from_stream(std::ifstream& stream,T& t)
    {
        stream.read(reinterpret_cast<char*>(&t),sizeof(T));
    }
    std::size_t file_size(const std::string& file_name) const
    {
        std::ifstream file(file_name.c_str(),std::ios::in | std::ios::binary);
        if (!file)
            return 0;
        file.seekg (0, std::ios::end);
        return static_cast<std::size_t>(file.tellg());
    }
    unsigned char* row(unsigned int row_index) const
    {
        return const_cast<unsigned char*>(&data_[(row_index * row_increment_)]);
    }
};

typedef image::rgb_t rgb_t;


#endif //__H__IMAGE__
