Shader = {
	Name = "test_postprocessing",
	Properties = {
		MainTex = {
		
		}	
	}

	SubShader =
	{
		Include = {
			"commona_unifrom_buffer.inc",
			"custom.inc"
		},	
	
		Text = [=[
		in vec3 aPos;
		uniform mat4 Transform;

		void main()
		{
			gl_Position = Transform * aPos;	
		}

		]=],
	
	}

	-- Any methods here, for custom behavior

	function OnLoad()
		Render:OnShaderLoad(
			self.Properties,
			self.SubShader.Include
		)	
	end
}
