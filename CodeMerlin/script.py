import customtkinter as ctk
from tkinter import filedialog
from openai import OpenAI
import threading
import os

ctk.set_appearance_mode("Light")
ctk.set_default_color_theme("blue")


class CodeMerlinApp(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("CodeMerlin")
        self.geometry("1000x600")

        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=1)

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=0)

        title_font = ctk.CTkFont(family="courier 10 pitch", size=50, weight="bold")
        header_font = ctk.CTkFont(family="Roboto", size=20, weight="normal")

        self.title_label = ctk.CTkLabel(self, text="Code Merlin", font=title_font)
        self.title_label.grid(row=0, column=0, columnspan=2, pady=(10, 0))

        self.input_label = ctk.CTkLabel(self, text="Input Code", font=header_font)
        self.input_label.grid(row=1, column=0, sticky="sw", padx=20, pady=(10, 0))

        self.input_text = ctk.CTkTextbox(self, width=400, corner_radius=0, border_width=3)
        self.input_text.grid(row=2, column=0, sticky="nsew", padx=20, pady=(5, 20))
        self.input_text.insert("0.0", "// Paste your code here...")

        self.output_label = ctk.CTkLabel(self, text="Merlin's Wisdom", font=header_font)
        self.output_label.grid(row=1, column=1, sticky="sw", padx=20, pady=(10, 0))

        self.output_text = ctk.CTkTextbox(self, width=400, corner_radius=0, border_width=3)
        self.output_text.grid(row=2, column=1, sticky="nsew", padx=20, pady=(5, 20))
        self.output_text.configure(state="disabled")

        self.load_button = ctk.CTkButton(self, text = "Load Code", command= self.load_file, width = 100, height = 24, corner_radius = 2, hover_color = "blue")
        self.load_button.grid(row = 1, column = 0, sticky="se", padx = 20, pady = (10,5))

        self.magic_button = ctk.CTkButton(
            self,
            text="Let Merlin work!",
            command=self.start_magic_thread,
            fg_color="green",
            hover_color="darkgreen",
            height=50,
            font=("Arial", 18, "bold"),
            corner_radius=3,
            border_width=0,
        )
        self.magic_button.grid(row=3, column=0, columnspan=2, pady=20)

        self.client = OpenAI(base_url="http://localhost:1234/v1", api_key="lm-studio")

    def load_file(self):
        try:
            file_path = filedialog.askopenfilename(title = "Select File", filetypes = (("Java Files", "*.java"), ("Python Files", "*.py"), ("C Files", "*.h"),
                                                                                       ("Lua Files", "*.lua"), ("HTML Files", "*.html"), ("JavaScript Files", "*.js"),
                                                                                       ("C++ Files", "*.cpp"),("All Files", "*.*")))
            if file_path:
                with open(file_path, "r") as f:
                    content = f.read()
                self.input_text.delete("0.0", "end")
                self.input_text.insert("0.0",content)

        except Exception as e:
            print(f"ERROR: file was unable to be opened: {e}")


    def start_magic_thread(self):
        self.magic_button.configure(state="disabled", text="Merlin is working...")
        self.output_text.configure(state="normal")
        self.output_text.delete("0.0", "end")

        threading.Thread(target=self.perform_magic, daemon=True).start()

    def perform_magic(self):
        aiInstructions = ("You are Merlin, a friendly and helpful software engineering assistant. "
                          "Your goal is to review code for bugs, large flaws, and performance issues. Your priorities are to look for large flaws first, then syntax errors/logical bugs/typos, and finally performance issues in code that makes it theoretically slow (poor Big O complexity). "
                          "You will respond in the following format: Summary: a brief explanation of what the code does, then the review, in which you will quote the code in which the error exists, and provide the fix; "
                          "if performance issues exsist, explain the Big-O complexity and how to improve it. Finally, provide full code snippets for whatever you fixed in proper markdown. "
                          "IF the code is bug-free and efficient, simply say so, do NOT invent problems.")
        try:
            code_snippet = self.input_text.get("0.0", "end")

            completion = self.client.chat.completions.create(
                model="fireball-meta-llama-3.2-8b-instruct-agent-003-128k-code-dpo",
                messages=[
                    {"role": "system",
                     "content": aiInstructions},
                    {"role": "user", "content": code_snippet}
                ],
                temperature=0.7,
                stream=True,
            )

            for chunk in completion:
                if chunk.choices[0].delta.content:
                    content = chunk.choices[0].delta.content

                    self.output_text.insert("end", content)
                    self.output_text.see("end")

        except Exception as e:
            self.output_text.insert("end", f"\nError: {str(e)}")
        finally:
            self.output_text.configure(state="disabled")
            self.magic_button.configure(state="normal", text = "Let Merlin work!")


if __name__ == "__main__":
    app = CodeMerlinApp()
    app.mainloop()