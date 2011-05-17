//Pra começar, só queremos a SDL.h e gl.h
#include <SDL/SDL.h>
#include <GL/gl.h>

int main(int argc, char *argv[])
{
	//Essas duas funções servem pra criar a nossa janela. A primeira inicializa
	//a SDL, enquanto que a segunda cria de fato a janela. Os parâmetros para a
	//segunda função são: largura, altura, bpp, modo.
	//	* largura, altura: as dimensões da janela que queremos criar
	//	* bpp: bits per pixel, a "resolução de cores" da janela. O valor 32
	//	significa que queremos uma janela RGBA com 8 bits pra cada componente
	//	* modo: como a janela deve ser criada. O valor SDL_OPENGL indica que a
	//	janela deve ser criada com suporte a OpenGL
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface *tela = SDL_SetVideoMode(300, 600, 32, SDL_OPENGL);

	//Ah, o loop principal. Jogue "false" em "rodando" e o programa para
	bool rodando = true;
	while (rodando)
	{
		//A primeira coisa a se fazer todo frame é verificar quais foram os
		//eventos gerados pelo sistema operacional no frame anterior. Por
		//"evento", entende-se:
		//	* Tecla pressionada/solta
		//	* Botão do mouse pressionado/solto
		//	* Mouse movimentado
		//	* Ícone "X" da janela clicado
		//	* Janela redimensionada
		//	* Outros eventos do sistema operacional que não nos interessam agora
		SDL_Event e;
		//A função SDL_PollEvent pega o primeiro evento não processado até
		//agora (são armazenados em uma fila). Ela retorna 0 quando não há mais
		//eventos pra serem processados e alguma coisa diferente de zero quando
		//há algo ainda pendente. Isso é usado pra tratar todos os eventos
		//_antes_ de fazer qualquer outra coisa no programa
		while (SDL_PollEvent(&e))
		{
			//Um evento bastante comum: tecla pressionada. Os códigos das teclas
			//como SDLK_ESCAPE, SDLK_UP etc. estão no arquivo SDL/SDL_keysym.h
			if (e.type == SDL_KEYDOWN)
				if ((e.key.keysym.sym == SDLK_ESCAPE) or (e.key.keysym.sym == SDLK_e))
					rodando = false;
			//Esse é o evento gerado quando o usuário clicou no "x" da janela.
			//Só clicar não faz nada, o programa é quem tem que decidir o que
			//fazer quando isso acontece. Aqui, só fechamos o programa direto
			if (e.type == SDL_QUIT)
				rodando = false;
		}

		//Esses comandos servem pra limpar a tela. O primeiro serve pra
		//especificar a cor que é pra ser usada pra limpar a tela, e não
		//precisaria ser chamada toda vez, pois ela seta a cor pra qual você
		//quer até que essa função seja chamada de novo. A segunda limpa de fato
		//a tela. O argumento indica o que deve ser limpo. Nesse caso, só
		//estamos limpando o "color buffer", mas há outros que usaremos em
		//outros projetos
		glClearColor(1,1,0,0);
		glClear(GL_COLOR_BUFFER_BIT);
		//Isso aqui é pra mostrar o buffer na tela. Até agora, todos os comandos
		//de desenhar alguma coisa foram aplicados em um buffer "escondido", pra
		//que não apareça na tela uma imagem pela metade. Essa função faz com
		//que o buffer que estava escondido seja mostrado e, a partir de agora,
		//os comandos de desenho vão pro buffer que aparecia antes. Depois troca
		//de novo e troca troca troca troca... Por isso que chama "double
		//buffering". A janela do SDL com OpenGL por padrão é criada com o
		//double buffer. Prático, né?
		SDL_GL_SwapBuffers();
		//Isso é só pra fazer o programa não gastar 100% de CPU pra uma coisa
		//tão simples. 
		SDL_Delay(10);
	}

	//Faz a SDL fechar todas as coisas dela
	SDL_Quit();

	//Tchau!
	return 0;
}
