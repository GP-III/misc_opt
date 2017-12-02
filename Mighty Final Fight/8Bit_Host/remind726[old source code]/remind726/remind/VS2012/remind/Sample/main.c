#include <system.h>
#include <ppu.h>
#include <input.h>
#include <image.h>

void __S(struct system_s *s, struct sp_chunk_s *t, struct rtkey_s *j, int si, int y)
{

}
int main (void)
{
	struct system_s *s = init_system(0,__S);
	//struct tnslen_s *i = tlread(s->ppu);
	struct bg_chunk_s *t = ksmread(s->ppu);
	//t->mask |= (TEB_VFLIP | TEB_HFLIP);
	ksmwrite(s->ppu, t);
	//fmskor(s->ppu, PPU_FT_VISABLE);
		fmskor(s->ppu, PPU_BG_VISABLE);
		//fmskor(s->ppu, PPU_UN_ALPHA);
		alloc_texure(s->video_handle, "bg2.png", 0);
		mesh_padding(s->ppu, detail_texture(s->video_handle, 0), 512, 0);
		
	show_window(s);
	system_swap(s);
}